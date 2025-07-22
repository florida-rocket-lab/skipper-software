#include "Converter.h"

uint16_t thrustToPwm(float T)
{
    T = constrain(T, -MAX_THRUST_N, MAX_THRUST_N);
    float n = (T + MAX_THRUST_N) / (2.0f * MAX_THRUST_N);
    return uint16_t(n * (ESC_MAX_US - ESC_MIN_US) + ESC_MIN_US);
}

uint16_t gimbalDegToPwm(float gimbalDeg)
{
    float servoDeg = constrain(gimbalDeg * GEAR_RATIO,
                               -GIMBAL_RANGE, GIMBAL_RANGE);
    float n = (servoDeg + GIMBAL_RANGE) / (2.0f * GIMBAL_RANGE);
    return uint16_t(n * (SERVO_MAX_US - SERVO_MIN_US) + SERVO_MIN_US);
}