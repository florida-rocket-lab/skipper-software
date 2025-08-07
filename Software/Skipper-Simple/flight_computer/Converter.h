#pragma once
#include <Arduino.h>          

constexpr float    MAX_THRUST_N   = 15.0f;
constexpr uint16_t ESC_MIN_US     = 1000;    
constexpr uint16_t ESC_MAX_US     = 2000;   

constexpr uint16_t SERVO_MIN_US   = 1000;   
constexpr uint16_t SERVO_MAX_US   = 21000;
constexpr float    GIMBAL_RANGE   = 8.5f;
constexpr double   GEAR_RATIO     = 1;    

uint16_t thrustToPwm(float thrustN);    
uint16_t gimbalDegToPwm(float angleDeg); 