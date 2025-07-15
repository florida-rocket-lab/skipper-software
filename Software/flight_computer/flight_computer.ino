

#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"
#include "Skipper.h"         

MPU6050 mpu;
bool initIMU()
{
  Wire.begin();
  mpu.initialize();
  return mpu.testConnection();
}

void setup()
{
  Serial.begin(115200);         
  Serial3.begin(38400);      

  if (!initIMU()) {
    Serial.println(F("MPU6050 init failed – halt"));
    while (true) delay(1000);
  }
  Serial.println(F("Flight computer ready (Serial3 @115200)"));
}

void loop()
{
  /* 1 — grab a raw IMU sample */
  int16_t axr, ayr, azr, gxr, gyr, gzr;
  mpu.getMotion6(&axr, &ayr, &azr, &gxr, &gyr, &gzr);

  /* 2 — pack frame */
  IMUFrame f;
  f.ax = axr / 16384.0f;
  f.ay = ayr / 16384.0f;
  f.az = azr / 16384.0f;
  f.gx = gxr / 131.0f;
  f.gy = gyr / 131.0f;
  f.gz = gzr / 131.0f;

  /* 3 — ship to Nano (raw IMU burst) */
  if (!uart_sendIMUFrame(Serial3, f))             
    Serial.println(F("UART send failed"));

  /* 4 — optional: receive command packet from Nano */
  uint8_t cmdBuf[128];
  size_t n = uart_recvCOBS(Serial3, cmdBuf, sizeof(cmdBuf));
  if (n) {
    Serial.print(F("CMD len=")); Serial.println(n);
    // …process command here…
  }

  delay(50);   // run @20 Hz
}
