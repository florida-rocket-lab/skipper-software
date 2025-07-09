#ifndef TEENSY_H
#define TEENSY_H

#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"
#include "datatypes.h"
#include "constants.h"     
#include <Servo.h>

#ifdef USE_LQI_STUB
  #include "skipper_lqi_stub.h"
#else
  #include "skipper_lqi.h"
#endif

class Teensy {
public:
  void init();
  void read_imu();
  
  void run_gnc();
  void printIMUData();
  IMUData getIMUData() const;

private:
  MPU6050     mpu;
  IMUData     imu_data;
  skipper_lqi controller;

  Servo       upperServo;
  Servo       lowerServo;
  static constexpr int thrustPin = TEENSY_ESC1_PIN;
  static constexpr int contraPin = TEENSY_ESC2_PIN;
};

#endif // TEENSY_H
