#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>

struct IMUFrame {
    float ax, ay, az, gx, gy, gz; // 24 bytes
};
#endif