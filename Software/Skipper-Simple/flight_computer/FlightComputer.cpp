#include "FlightComputer.h"
#include <Wire.h>

static constexpr float  kA = 1.f / 16384.f;
static constexpr float  kG = (1.f / 131.f) * (PI / 180.f);
static constexpr uint32_t CTRL_PERIOD_US = 50;          // 200 Hz
static constexpr float SEA_LVL_hPa       = 1013.25f;

//  Hard-coded phase durations (ms) 
static constexpr uint32_t COUNTDOWN_MS = 5000;
static constexpr uint32_t ASCENT_MS    = 5000;
static constexpr uint32_t HOVER_MS     = 2000;
static constexpr uint32_t DESCENT_MS   = 5000;

// Private plant state (12-D)
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
    return true;
}

// Main Loop
void FlightComputer::update() {
    // Handle incoming commands (may change phase) 
    handleRadioCmd();
    handleSerialCmd();

    uint32_t now   = millis();
    uint32_t since = now - phaseStartMs;

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
        // Linear ramp from 1 m to 0 m over 5s 
        float progress = (float)since / (float)DESCENT_MS;
        altRef = max(0.0f, 1.0f * (1.0f - progress));
        if (since >= DESCENT_MS) {
            fcPhase      = FlightPhase::DISARMED;
            phaseStartMs = now;
            disarmMotors();
            setLED(true, false);                         // green solid
        }
        break;
    }

    case FlightPhase::DISARMED:
        altRef = 0.0f;
        break;
    }

    // sensors 
    IMURaw r   = readIMU();
    float alt  = bmp.readAltitude(SEA_LVL_hPa);
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
        buildPlantState(r, alt);
        pushState();
        if (micros() - lastCtrlUs >= CTRL_PERIOD_US) {
            lastCtrlUs += CTRL_PERIOD_US;
            runController();
        }
    }
}

// Init 
bool FlightComputer::initIMU() {
    Wire.begin(); Wire.setClock(1'000'000);
    mpu.initialize();
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    if (!mpu.testConnection()) return false;
    mpu.CalibrateAccel(6); mpu.CalibrateGyro(6);
    return true;
}

bool FlightComputer::initBaro()  { 
    return bmp.begin(0x77); 
}

bool FlightComputer::initRadio() {
    if (!radio.begin()) return false;
    radio.setChannel(RADIO_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.openWritingPipe(RADIO_ADDRESS);
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
void FlightComputer::buildPlantState(const IMURaw& r, float alt) {
    velX += r.ax*kA*9.80665f*0.005f;
    velY += r.ay*kA*9.80665f*0.005f;
    auto&s = plantState.v;
    s[0]=alt; 
    s[1]=0; 
    s[2]=0; 
    s[3]=0;
    s[4]=velX; s[5]=velY;
    s[6]=ahrs.getRollRadians();
    s[7]=ahrs.getPitchRadians();
    s[8]=ahrs.getYawRadians();
    s[9]=r.gx*kG; 
    s[10]=r.gy*kG; 
    s[11]=r.gz*kG;
}
void FlightComputer::pushState() {
    memcpy(ctrl.skipper_lqi_for_export_U.imu_state_in,
           plantState.v, sizeof(plantState.v));
}

/* ───── Control / actuation ─────────────────────────────────────────────── */
void FlightComputer::runController() {
    /* Feed reference vector */
    ctrl.skipper_lqi_for_export_U.reference[0] = altRef;   // z
    ctrl.skipper_lqi_for_export_U.reference[1] = 0.0f;     // x
    ctrl.skipper_lqi_for_export_U.reference[2] = 0.0f;     // y

    ctrl.step();
    const auto& y = ctrl.skipper_lqi_for_export_Y;

    pwmEsc      = thrustToPwm(y.thrust);
    rollDegCmd  = constrain(y.upper_gimbal_angle, -30.f, 30.f);
    pitchDegCmd = constrain(y.lower_gimbal_angle, -30.f, 30.f);
    pwmUp       = gimbalDegToPwm(rollDegCmd);
    pwmLo       = gimbalDegToPwm(pitchDegCmd);

    esc1.writeMicroseconds(pwmEsc);
    esc2.writeMicroseconds(pwmEsc);
    servoUp.writeMicroseconds(pwmUp);
    servoLo.writeMicroseconds(pwmLo);
}
void FlightComputer::disarmMotors() { esc1.writeMicroseconds(1000); esc2.writeMicroseconds(1000); }

/* ───── Telemetry / logging ─────────────────────────────────────────────── */
void FlightComputer::sendTelemetry(const IMURaw& r, float alt) {
    IMUFrame f{ r.ax*kA, r.ay*kA, r.az*kA, r.gx*kG, r.gy*kG, r.gz*kG };
    rf_sendIMU(radio, f);
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

/* ───── Command handling ────────────────────────────────────────────────── */
void FlightComputer::handleRadioCmd() {
    if (!radio.available()) return;
    char c; radio.read(&c, 1);
    if (c=='A' && fcPhase==FlightPhase::IDLE)        { fcPhase=FlightPhase::COUNTDOWN; phaseStartMs=millis(); }
    if (c=='L' && fcPhase<=FlightPhase::DESCENT)     { fcPhase=FlightPhase::DESCENT;   phaseStartMs=millis(); }
}
void FlightComputer::handleSerialCmd() {
    if (!Serial.available()) return;
    char c = tolower(Serial.read());
    if ((c=='a') && fcPhase==FlightPhase::IDLE)      { fcPhase=FlightPhase::COUNTDOWN; phaseStartMs=millis(); }
    if ((c=='l') && fcPhase<=FlightPhase::DESCENT)   { fcPhase=FlightPhase::DESCENT;   phaseStartMs=millis(); }
}

/* ───── LED helper ──────────────────────────────────────────────────────── */
void FlightComputer::setLED(bool g, bool r) {
    digitalWrite(TEENSY_SUCCESS_PIN, g);
    digitalWrite(TEENSY_FAIL_PIN,    r);
}
