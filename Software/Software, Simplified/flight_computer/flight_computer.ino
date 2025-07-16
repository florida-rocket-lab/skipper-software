/*  ───────────── flight_computer.ino  (Teensy 4.0) ─────────────
 *  Teensy TX3 (pin 14) → Nano RX 2   (SoftwareSerial)
 *  Teensy RX3 (pin 15) ← Nano TX 3
 *  Baud: 38 400 baud   (match teen_link.begin(38400) on the Nano)
 *  --------------------------------------------------------------------- */

#include <Wire.h>
#include "MPU6050.h"
#include "link_frame.h"          // ⇦ your unified frame / CRC header

/* ─── IMU config ─────────────────────────────────────────────── */
MPU6050 mpu;
constexpr float ACC_SF = 16384.0f;   // 2 g full‑scale
constexpr float GYR_SF =   131.0f;   // 250 dps full‑scale
/* ────────────────────────────────────────────────────────────── */

void setup()
{
  Serial.begin(115200);            // USB console
  Serial3.begin(38400);            // telemetry link (pins 14/15)

  Wire.begin();
  delay(100);

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println(F("MPU6050 connection failed!"));
    while (true) delay(1000);
  }
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  mpu.setFullScaleGyroRange (MPU6050_GYRO_FS_250);

  Serial.println(F("Flight computer ready"));
}

void sendIMU(const IMUFrame& f)
{
  const uint8_t* p = reinterpret_cast<const uint8_t*>(&f);

  Serial3.write(START);            // 0xAA
  Serial3.write(FRAME_LEN);        // 25 bytes: 24 payload + CRC
  Serial3.write(p, sizeof(IMUFrame));
  Serial3.write(crc8(p, sizeof(IMUFrame)));
}

void loop()
{
  /* 1. read raw motion data */
  int16_t axr, ayr, azr, gxr, gyr, gzr;
  mpu.getMotion6(&axr, &ayr, &azr, &gxr, &gyr, &gzr);

  /* 2. scale to physical units */
  IMUFrame f = {
    axr / ACC_SF, ayr / ACC_SF, azr / ACC_SF,
    gxr / GYR_SF, gyr / GYR_SF, gzr / GYR_SF
  };

  /* 3. transmit */
  sendIMU(f);

  /* 4. (optional) print to USB for sanity */
  Serial.print(F("ax:")); Serial.print(f.ax,3);
  Serial.print(F(" ay:")); Serial.print(f.ay,3);
  Serial.print(F(" az:")); Serial.print(f.az,3);
  Serial.print(F("  gx:")); Serial.print(f.gx,3);
  Serial.print(F(" gy:"));  Serial.print(f.gy,3);
  Serial.print(F(" gz:"));  Serial.println(f.gz,3);

  delay(100);                        // 10 Hz update
}
