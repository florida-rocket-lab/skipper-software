#include "Converter.h"

uint16_t thrustToPwm_fromLbf(float lbf){
  float n = constrain(lbf / MAX_THRUST_LBF, 0.f, 1.f);
  return uint16_t(ESC_MIN_US + n * (ESC_MAX_US - ESC_MIN_US));
}

uint16_t gimbalDegToPwm(float gimbalDeg)
{
    float servoDeg = constrain(gimbalDeg * GEAR_RATIO, -GIMBAL_RANGE, GIMBAL_RANGE);
    if (fabs(servoDeg) < 0.1f) servoDeg = 0.0f; // Deadband
    float n = (servoDeg + GIMBAL_RANGE) / (2.0f * GIMBAL_RANGE);
    return static_cast<uint16_t>(round(n * (SERVO_MAX_US - SERVO_MIN_US) + SERVO_MIN_US));
}