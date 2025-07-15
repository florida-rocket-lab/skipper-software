#pragma once
#include <Arduino.h>

#pragma pack(push,1)
struct IMUFrame {            // 24 bytes
  float ax, ay, az;
  float gx, gy, gz;
};
#pragma pack(pop)
static_assert(sizeof(IMUFrame)==24, "IMUFrame wrong size");

inline uint8_t crc8(const uint8_t* d, uint8_t len) {
  uint8_t c=0;
  while (len--) {
    c ^= *d++;
    for (uint8_t i=0;i<8;i++)
      c = (c & 0x80) ? (c<<1)^0x07 : (c<<1);
  }
  return c;
}

/* framing: 0xAA  len  payload[24]  crc
 * len = 25 (=payload+crc)  — fits in 1 byte
 */
constexpr uint8_t START = 0xAA;
constexpr uint8_t FRAME_LEN = sizeof(IMUFrame)+1;   // 25
