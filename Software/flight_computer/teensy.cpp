#include "teensy.h"

// full-scale ranges and scale-factors
#define ACCEL_FS   MPU6050_ACCEL_FS_2
#define GYRO_FS    MPU6050_GYRO_FS_250
#define ACCEL_SF   16384.0   // 2 g → 16384 LSB/g
#define GYRO_SF    131.0     // 250 dps → 131 LSB/dps

void Teensy::init() {
  Serial.begin(9600);
  Wire.begin();
  delay(100);

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1) { delay(1000); }
  }

  mpu.setFullScaleAccelRange(ACCEL_FS);
  mpu.setFullScaleGyroRange(GYRO_FS);

  controller.initialize();

  upperServo.attach(thrustPin);
  lowerServo.attach(contraPin);

  Serial.println("Teensy + IMU initialized.");
}

void Teensy::read_imu() {
  int16_t ax_raw, ay_raw, az_raw;
  int16_t gx_raw, gy_raw, gz_raw;

  mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

  float ax = ax_raw / ACCEL_SF;
  float ay = ay_raw / ACCEL_SF;
  float az = az_raw / ACCEL_SF;

  float gx = gx_raw / GYRO_SF;
  float gy = gy_raw / GYRO_SF;
  float gz = gz_raw / GYRO_SF;

  new (&imu_data.acc) Vector3(ax, ay, az);
  new (&imu_data.gyr) Vector3(gx, gy, gz);
}

void Teensy::printIMUData() {
  const auto& imu = imu_data;

  Serial.print(F("Accel [x, y, z]: "));
  Serial.print(imu.acc.x); Serial.print(F(", "));
  Serial.print(imu.acc.y); Serial.print(F(", "));
  Serial.println(imu.acc.z);

  Serial.print(F("Gyro  [x, y, z]: "));
  Serial.print(imu.gyr.x); Serial.print(F(", "));
  Serial.print(imu.gyr.y); Serial.print(F(", "));
  Serial.println(imu.gyr.z);
}

void Teensy::run_gnc() {
  controller.step();

  auto& out         = controller.getRTM()->skipper_lqi_Y;
  float thrust      = out.thrust;
  float contra_spin = out.counter_rotating;
  float upper_ang   = out.upper_gimbal_angle;
  float lower_ang   = out.lower_gimbal_angle;

  upperServo.write(upper_ang);
  lowerServo.write(lower_ang);

  // mapThrustToPwm is provided by constants.h :contentReference[oaicite:1]{index=1}
  analogWrite(thrustPin, mapThrustToPwm(thrust));
  analogWrite(contraPin, mapThrustToPwm(contra_spin));
}

IMUData Teensy::getIMUData() const {
  return imu_data;
}