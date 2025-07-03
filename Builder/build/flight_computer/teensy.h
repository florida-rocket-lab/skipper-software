#ifndef TEENSY_H
#define TEENSY_H

#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"
#include "datatypes.h"   
#include "skipper_lqi.h"
#include <Servo.h>


class Teensy {
public:
  void init();
  void read_imu();
  void run_gnc();
   void printIMUData();

private:
  MPU6050 mpu;
  IMUData imu_data;
  skipper_lqi controller;

  Servo upperServo, lowerServo;
  static constexpr int thrustPin = TEENSY_ESC1_PIN;
  static constexpr int contraPin = TEENSY_ESC2_PIN;

};

#endif  
