

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
#include <MadgwickAHRS.h>



#include "packet.h"          
#include "communication.h"  
#include "Converter.h"      
#include "constants.h"       
#include "skipper_lqi_for_export.h"


#ifdef printf
#undef printf                 // RF24 macro clash
#endif

MPU6050                mpu;
skipper_lqi_for_export ctrl;
Madgwick filter; // used to get rotational data from imu

Servo servoUpper;
Servo servoLower;
Servo esc1;
Servo esc2;

constexpr uint32_t CTRL_PERIOD_MS = 5;  
uint32_t tLastCtrl = 0;

bool initIMU() {
  Wire.begin();
  Wire.setClock(1000000);                // 1 MHz so fast‑mode plus
  mpu.initialize();
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  mpu.setFullScaleGyroRange (MPU6050_GYRO_FS_250);
  return mpu.testConnection();
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Serial3.begin(115200);

  if (!initIMU()) {
    Serial.println(F("MPU6050 init failed – halt"));
    while (true) delay(1000);
  }

  ctrl.initialize();

  esc1.attach(TEENSY_ESC1_PIN);
  esc2.attach(TEENSY_ESC2_PIN);
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  delay(2000);

  servoUpper.attach(TEENSY_UPPER_GIMBAL_PIN, SERVO_MIN_US, SERVO_MAX_US);
  servoLower.attach(TEENSY_LOWER_GIMBAL_PIN, SERVO_MIN_US, SERVO_MAX_US);

  mpu.CalibrateAccel(6); 
  mpu.CalibrateGyro(6);

  Serial.println(F("Flight computer ready"));
}
void loop() {
  // raw IMU 
  int16_t axc, ayc, azc, gxc, gyc, gzc;
  mpu.getMotion6(&axc,&ayc,&azc,&gxc,&gyc,&gzc);

  constexpr float kA = 1.0f / 16384.0f;          // g   to m s‑2
  constexpr float kG = 1.0f / 131.0f;            // °/s to rad s‑1

  //Madgwick filter
  filter.updateIMU(gxc*kG, gyc*kG, gzc*kG,
                   axc*kA*9.80665f, ayc*kA*9.80665f, azc*kA*9.80665f);

  float roll  = filter.getRollRadians();
  float pitch = filter.getPitchRadians();
  float yaw   = filter.getYawRadians();

  Serial.printf("Raw: %d,%d,%d,%d,%d,%d\n", axc, ayc, azc, gxc, gyc, gzc);
  Serial.printf("RPY: %.2f,%.2f,%.2f\n", roll*57.3f, pitch*57.3f, yaw*57.3f); 

  //build 12‑state vector (Simulink order) ───────────
    // 0 altitude (m, +up)      
    //1 cross‑range X          
    //2 down‑range  Y          
    //3..       
    //5 velocities   
    //6 roll      (rad)
    // 7 pitch
    // 8 yaw
  auto &u = ctrl.skipper_lqi_for_export_U;
  float *s = u.imu_state_in;


  s[6]  = roll;
  s[7]  = pitch;
  s[8]  = yaw;
  s[9]  = gxc * kG;
  s[10] = gyc * kG;
  s[11] = gzc * kG;

  

  // reference vector, starts at 0 then steps up to 1m
  u.reference[1] = 0.0f;          // X
  u.reference[2] = 0.0f;          // Y
  u.reference[0] = (millis() > 5000) ? -1.0f : 0.0f;   // Z

  /// controller tick (200 Hz)
  if (uint32_t now = millis(); now - tLastCtrl >= CTRL_PERIOD_MS) {
    tLastCtrl = now;

    ctrl.step();
    const auto &y = ctrl.skipper_lqi_for_export_Y;

    uint16_t pwmEsc   = thrustToPwm(y.thrust);            

    const real_T *cmd = ctrl.skipper_lqi_for_export_Y.control;


    float pitchDeg =  y.upper_gimbal_angle  * 57.2958f;   // rad to degree
    float rollDeg  = -y.lower_gimbal_angle  * 57.2958f;   // sign‑flip
    uint16_t pwmUpper = gimbalDegToPwm(pitchDeg);
    uint16_t pwmLower = gimbalDegToPwm(rollDeg);

    Serial.printf("State: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11]);
    Serial.printf("Ctrl: %.2f,%.2f,%.2f\n", y.thrust, y.upper_gimbal_angle*57.3f, y.lower_gimbal_angle*57.3f);

    esc1.writeMicroseconds(pwmEsc);
    esc2.writeMicroseconds(pwmEsc);
    servoUpper.writeMicroseconds(pwmUpper);
    servoLower.writeMicroseconds(pwmLower);

    
    static bool header = true;
    if (header) { Serial.println(F("ESC1,ESC2,SERVO_UP,SERVO_LOW")); header = false; }
    Serial.print(pwmEsc);   Serial.print(',');
    Serial.print(pwmEsc);   Serial.print(',');
    Serial.print(pwmUpper); Serial.print(',');
    Serial.println(pwmLower);

    
    Serial.printf("RPY %.1f %.1f %.1f°  CMD %.2f %.2f %.2f %.2f\n",
                  roll*57.3f, pitch*57.3f, yaw*57.3f,
                  cmd[0], cmd[1], cmd[2], cmd[3]);
   
  }

  // telemetry
  IMUFrame f;
  f.ax = axc*kA;  f.ay = ayc*kA;  f.az = azc*kA;
  f.gx = gxc*kG;  f.gy = gyc*kG;  f.gz = gzc*kG;
  uart_sendIMUFrame(Serial3, f);
}
