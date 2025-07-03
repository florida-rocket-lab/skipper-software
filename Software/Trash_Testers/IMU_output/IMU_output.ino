#include <Wire.h>
#include <MPU6050.h>  

MPU6050 mpu;

#define ACCEL_FS   MPU6050_ACCEL_FS_2
#define GYRO_FS    MPU6050_GYRO_FS_250
#define ACCEL_SF   16384.0   
#define GYRO_SF    131.0      

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1) { delay(1000); }
  }

  mpu.setFullScaleAccelRange(ACCEL_FS);
  mpu.setFullScaleGyroRange(GYRO_FS);

  Serial.println("MPU6050 ready.");
}

void loop() {
  int16_t ax_raw, ay_raw, az_raw;
  int16_t gx_raw, gy_raw, gz_raw;
  
  mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

  // convert to physical units
  float ax = ax_raw / ACCEL_SF;    // G’s
  float ay = ay_raw / ACCEL_SF;
  float az = az_raw / ACCEL_SF;

  float gx = gx_raw / GYRO_SF;     // °/s
  float gy = gy_raw / GYRO_SF;
  float gz = gz_raw / GYRO_SF;

  Serial.print("Acc (g): ");
    Serial.print(ax, 3); Serial.print(", ");
    Serial.print(ay, 3); Serial.print(", ");
    Serial.print(az, 3);
  Serial.print("  |  Gyro (°/s): ");
    Serial.print(gx, 2); Serial.print(", ");
    Serial.print(gy, 2); Serial.print(", ");
    Serial.println(gz, 2);

  delay(100);  // 
}
