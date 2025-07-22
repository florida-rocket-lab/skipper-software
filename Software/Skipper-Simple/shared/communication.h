#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <RF24.h>
#include "packet.h"
#include "constants.h"

bool rf_sendIMU(RF24& radio, const IMUFrame& frame);
bool rf_recvIMU(RF24& radio, IMUFrame& frame);
bool uart_sendIMUFrame(Stream& s, const IMUFrame& frame);
size_t uart_recvCOBS(Stream& s, uint8_t* buf, size_t len);
uint8_t compute_crc8(const uint8_t* data, size_t len);

#endif