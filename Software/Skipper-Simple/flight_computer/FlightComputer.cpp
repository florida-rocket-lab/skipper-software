#include "FlightComputer.h"
#include <Wire.h>

static constexpr float  kA = 1.f / 16384.f;
static constexpr float  kG = (1.f / 131.f) * (PI / 180.f);
static constexpr uint32_t CTRL_PERIOD_US = 25'000;    // 40Hz
static constexpr float SEA_LVL_hPa       = 1013.25f;

//  Hard-coded phase durations 
static constexpr uint32_t COUNTDOWN_MS = 10000;
static constexpr uint32_t ASCENT_MS    = 5000;
static constexpr uint32_t HOVER_MS     = 2000;
static constexpr uint32_t DESCENT_MS   = 5000;

constexpr float M2FT     = 3.2808399f;
constexpr float MPS2FTPS = 3.2808399f;
constexpr float RAD2DEG  = 57.2957795f;



// Private plant state 
struct State12 { float v[12]; };
static State12 plantState;

bool FlightComputer::begin() {
    pinMode(TEENSY_SUCCESS_PIN, OUTPUT);
    pinMode(TEENSY_FAIL_PIN,    OUTPUT);
    pinMode(TEENSY_BUZZER_PIN,  OUTPUT);

    if (!initIMU())   { Serial.println("IMU fail");   return false; }
    if (!initBaro())  { Serial.println("BMP280 fail");return false; }
    if (!initRadio()) { Serial.println("RF24 fail");  return false; }
    if (!initSD())    { Serial.println("SD fail");    return false; }

    ctrl.initialize();
    initActuators();

    fcPhase      = FlightPhase::IDLE;
    phaseStartMs = millis();
    setLED(true, false);                    // green solid

    lastCtrlUs = micros(); 
    return true;
}

// Main Loop
void FlightComputer::update() {
    // Handle incoming commands (may change phase) 
    handleRadioCmd();
    handleSerialCmd();


    uint32_t now   = millis();
    uint32_t since = now - phaseStartMs;

    // HARD TIMEOUT
    if (runActive && (int32_t)(now - runStopMs) >= 0) {
        runActive = false;
        fcPhase   = FlightPhase::DISARMED;
        disarmMotors();             
        setLED(true, false);      
    }

    switch (fcPhase) {
    case FlightPhase::IDLE:
        break;

    case FlightPhase::COUNTDOWN:
        setLED(true, true);                              // red + green solid
        tone(TEENSY_BUZZER_PIN, 1000 + (5 - since / 1000) * 200);
        if (since >= COUNTDOWN_MS) {
            noTone(TEENSY_BUZZER_PIN);
            fcPhase      = FlightPhase::ASCENT;
            phaseStartMs = now;
            altRef       = 1.0f;                         // 1 m target
            setLED(false, true);                         // red solid
        }
        break;

    case FlightPhase::ASCENT:
        altRef = 1.0f;                                   // hold target
        if (since >= ASCENT_MS) {
            fcPhase      = FlightPhase::HOVER;
            phaseStartMs = now;
        }
        break;

    case FlightPhase::HOVER:
        altRef = 1.0f;
        if (since >= HOVER_MS) {
            fcPhase      = FlightPhase::DESCENT;
            phaseStartMs = now;
        }
        break;

    case FlightPhase::DESCENT: {
        float progress = (float)since / (float)DESCENT_MS;
        altRef = max(0.0f, 1.0f * (1.0f - progress));
        if (since >= DESCENT_MS) {
            fcPhase      = FlightPhase::DISARMED;
            phaseStartMs = now;
            disarmMotors();
            setLED(true, false);
            runActive = false;        
        }
        break;
    }

    case FlightPhase::DISARMED:
        altRef = 0.0f;
        break;
    }

    // sensors 
    IMURaw r   = readIMU();
    float rawAlt  = bmp.readAltitude(SEA_LVL_hPa) - alt0;
    altFilt = 0.95f*altFilt + 0.05f*rawAlt;   // simple IIR
    float alt = altFilt;  
    updateAHRS(r);

    //log telem 
    sendTelemetry(r, alt);
    logFrame(r, alt);
    if (now - lastFlushMs > 1000) { if (logFile) logFile.flush(); lastFlushMs = now; }

// control loop
    if (fcPhase == FlightPhase::ASCENT ||
        fcPhase == FlightPhase::HOVER  ||
        fcPhase == FlightPhase::DESCENT)
    {
        if (micros() - lastCtrlUs >= CTRL_PERIOD_US) {
        lastCtrlUs += CTRL_PERIOD_US;
        const float dt = CTRL_PERIOD_US * 1e-6f;     // 0.025 s
        buildPlantState(r, alt, dt); 
        pushState();
        runController();
        }   
    }
}

// Init 
bool FlightComputer::initIMU() {
    Wire.begin(); 
    Wire.setClock(1'000'000);
    mpu.initialize();
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    if (!mpu.testConnection()) return false;
    mpu.CalibrateAccel(6); mpu.CalibrateGyro(6);
    return true;
}

bool FlightComputer::initBaro() {
  if (!bmp.begin(0x77)) return false;
  delay(100);
  alt0 = bmp.readAltitude(SEA_LVL_hPa);   // baseline

  altFilt = 0;
  return true;
}

bool FlightComputer::initRadio() {
    if (!radio.begin()) return false;
    radio.setChannel(RADIO_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.openWritingPipe(RADIO_ADDRESS);
    radio.openReadingPipe(1, RADIO_ADDRESS);   
    radio.startListening();
    return true;
}

bool FlightComputer::initSD() {
    if (!SD.begin(BUILTIN_SDCARD)) return false;
    SD.remove("flight.csv");
    logFile = SD.open("flight.csv", FILE_WRITE);
    if (!logFile) return false;
    logFile.println("t,alt,ax,ay,az,gx,gy,gz,escPWM,rollDeg,pitchDeg");
    return true;
}
void FlightComputer::initActuators() {
    esc1.attach(TEENSY_ESC1_PIN);  esc2.attach(TEENSY_ESC2_PIN);
    servoUp.attach(TEENSY_UPPER_GIMBAL_PIN, SERVO_MIN_US, SERVO_MAX_US);
    servoLo.attach(TEENSY_LOWER_GIMBAL_PIN, SERVO_MIN_US, SERVO_MAX_US);
    disarmMotors();
}

FlightComputer::IMURaw FlightComputer::readIMU() {
    IMURaw r; 
    mpu.getMotion6(&r.ax,&r.ay,&r.az,&r.gx,&r.gy,&r.gz); 
    return r;
}
void FlightComputer::updateAHRS(const IMURaw& r) {
    ahrs.updateIMU(r.gx*kG,r.gy*kG,r.gz*kG, r.ax*kA*9.80665f,r.ay*kA*9.80665f,r.az*kA*9.80665f);
}
void FlightComputer::buildPlantState(const FlightComputer::IMURaw& r,
                                     float alt_m, float dt) {
  velX += r.ax * kA * 9.80665f * dt;   
  velY += r.ay * kA * 9.80665f * dt;  
  const float alt_ft  = alt_m * M2FT;
  const float vX_ftps = velX * MPS2FTPS;  
  const float vY_ftps = velY * MPS2FTPS;

  auto& s = plantState.v;
  s[0]  = alt_ft;         // altitude
  s[1]  = 0.0f;           // crossrange
  s[2]  = 0.0f;           // downrange  
  s[3]  = vX_ftps;        // lateral_vel
  s[4]  = vY_ftps;        // longitudinal_vel
  s[5]  = 0.0f;           // directional_vel
  s[6]  = ahrs.getRollRadians()  * RAD2DEG;
  s[7]  = ahrs.getPitchRadians() * RAD2DEG;
  s[8]  = ahrs.getYawRadians()   * RAD2DEG;
  s[9]  = (r.gx * kG) * RAD2DEG; 
  s[10] = (r.gy * kG) * RAD2DEG;
  s[11] = (r.gz * kG) * RAD2DEG;
}
void FlightComputer::pushState() {
    memcpy(ctrl.skipper_lqi_for_export_U.imu_state_in,
           plantState.v, sizeof(plantState.v));
}

void FlightComputer::setReferenceFeet(float alt_ft, float cross_ft, float down_ft){
  ctrl.skipper_lqi_for_export_U.reference[0] = cross_ft; // x
  ctrl.skipper_lqi_for_export_U.reference[1] = down_ft;  // y
  ctrl.skipper_lqi_for_export_U.reference[2] = alt_ft;   // z
}


// control 
void FlightComputer::runController() {
  setReferenceFeet(altRef * M2FT, 0.0f, 0.0f);

  ctrl.step();
  const auto& y = ctrl.skipper_lqi_for_export_Y;

  pwmEsc      = thrustToPwm_fromLbf(y.thrust);
  rollDegCmd  = constrain(y.upper_gimbal_angle, -GIMBAL_RANGE, GIMBAL_RANGE);
  pitchDegCmd = constrain(y.lower_gimbal_angle, -GIMBAL_RANGE, GIMBAL_RANGE);
  pwmUp       = gimbalDegToPwm(rollDegCmd);
  pwmLo       = gimbalDegToPwm(pitchDegCmd);

  esc1.writeMicroseconds(pwmEsc);
  esc2.writeMicroseconds(pwmEsc);
  servoUp.writeMicroseconds(pwmUp);
  servoLo.writeMicroseconds(pwmLo);

  if (fcPhase == FlightPhase::DISARMED){
    pwmEsc = ESC_MIN_US;
    esc1.writeMicroseconds(pwmEsc);
    esc2.writeMicroseconds(pwmEsc);
  }

}

void FlightComputer::disarmMotors() {
  pwmEsc = ESC_MIN_US;
  esc1.writeMicroseconds(pwmEsc);
  esc2.writeMicroseconds(pwmEsc);
  rollDegCmd = pitchDegCmd = 0.0f;                 
  pwmUp = pwmLo = (SERVO_MIN_US + SERVO_MAX_US)/2;
  servoUp.writeMicroseconds(pwmUp);
  servoLo.writeMicroseconds(pwmLo);
}


void FlightComputer::sendTelemetry(const IMURaw& imu, float alt_m) {
  TelemetryV1 t{};
  t.ax_mg = int16_t(imu.ax * 1000.0f);
  t.ay_mg = int16_t(imu.ay * 1000.0f);
  t.az_mg = int16_t(imu.az * 1000.0f);
  auto dps10 = [](float rad_s){ return int16_t(rad_s * 57.2957795f * 10.0f); };
  t.gx_dps10 = dps10(imu.gx);
  t.gy_dps10 = dps10(imu.gy);
  t.gz_dps10 = dps10(imu.gz);
  t.alt_cm   = int16_t(alt_m * 100.0f);
  t.esc_us   = pwmEsc;
  t.servo_x_us = pwmUp;
  t.servo_y_us = pwmLo;
  t.phase    = uint8_t(fcPhase);
  t.ver      = 1;
  t.thrust_milli = int16_t(ctrl.skipper_lqi_for_export_Y.thrust * 1000.f);


  rf_sendTelemetry(radio, t);  
}

void FlightComputer::logFrame(const IMURaw& r, float alt) {
    if (!logFile) return;
    logFile.print(millis()); logFile.print(',');
    logFile.print(alt, 2);   logFile.print(',');
    logFile.print(r.ax*kA, 4); logFile.print(',');
    logFile.print(r.ay*kA, 4); logFile.print(',');
    logFile.print(r.az*kA, 4); logFile.print(',');
    logFile.print(r.gx*kG, 4); logFile.print(',');
    logFile.print(r.gy*kG, 4); logFile.print(',');
    logFile.print(r.gz*kG, 4); logFile.print(',');
    logFile.print(pwmEsc);     logFile.print(',');
    logFile.print(rollDegCmd, 2); logFile.print(',');
    logFile.println(pitchDegCmd, 2);
    if (logFile.position() % 512 == 0) logFile.flush();
}


// commands 
void FlightComputer::handleRadioCmd() {
    if (!radio.available()) return;
    char c; radio.read(&c, 1);

    if (c=='A' && fcPhase==FlightPhase::IDLE) {
        fcPhase=FlightPhase::COUNTDOWN; 
        phaseStartMs=millis();
        runActive = true;
        runStopMs = phaseStartMs + RUN_WINDOW_MS;   // 15 s window starts now
    }
    if (c=='L' && fcPhase<=FlightPhase::DESCENT)     { fcPhase=FlightPhase::DESCENT;   phaseStartMs=millis(); }
}
void FlightComputer::handleSerialCmd() {
    if (!Serial.available()) return;
    char c = tolower(Serial.read());
    if ((c=='a') && fcPhase==FlightPhase::IDLE) {
        fcPhase=FlightPhase::COUNTDOWN; 
        phaseStartMs=millis();
        runActive = true;
        runStopMs = phaseStartMs + RUN_WINDOW_MS;
    }
    if ((c=='l') && fcPhase<=FlightPhase::DESCENT)   { fcPhase=FlightPhase::DESCENT;   phaseStartMs=millis(); }
}

void FlightComputer::setLED(bool g, bool r) {
    digitalWrite(TEENSY_SUCCESS_PIN, g);
    digitalWrite(TEENSY_FAIL_PIN,    r);
}
