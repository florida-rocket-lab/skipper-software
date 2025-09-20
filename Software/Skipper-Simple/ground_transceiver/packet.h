#ifndef SKIPPER_PACKET_H
#define SKIPPER_PACKET_H

#include <Arduino.h>

struct IMUFrame {
  float ax, ay, az, gx, gy, gz;
};

#pragma pack(push,1)
struct TelemetryV1 {
  int16_t ax_mg, ay_mg, az_mg;
  int16_t gx_dps10, gy_dps10, gz_dps10;
  int16_t alt_cm;
  uint16_t esc_us, servo_x_us, servo_y_us;
  uint8_t  phase, ver;
  int16_t  thrust_milli;   // NEW: lbf * 1000
};
#pragma pack(pop)
static_assert(sizeof(TelemetryV1)==24, "TelemetryV1 must be 24 bytes");

#endif // SKIPPER_PACKET_H
