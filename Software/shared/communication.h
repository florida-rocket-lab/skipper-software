#pragma once
#include <Arduino.h>
#include <RF24.h>
#include "packet.h"   
#include "cobs.h"

bool rf_sendIMU (RF24& radio, const IMUFrame& imu);
bool rf_recvIMU (RF24& radio, IMUFrame& imu);

bool uart_sendCOBS(Stream& s, const uint8_t* buf, size_t n);
size_t uart_recvCOBS(Stream& s, uint8_t* out, size_t max);

bool uart_sendIMUFrame(Stream& s, const IMUFrame& imu);   
