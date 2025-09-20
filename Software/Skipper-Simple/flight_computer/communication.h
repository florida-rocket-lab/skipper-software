#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <RF24.h>
#include "packet.h"
#include "constants.h"

bool rf_sendPacket(RF24& radio, const void* payload, uint8_t len);
bool rf_recvPacket(RF24& radio, uint8_t* payload, uint8_t expect_len);

bool rf_sendTelemetry(RF24& radio, const TelemetryV1& t);
bool rf_recvTelemetry(RF24& radio, TelemetryV1& t);

uint8_t compute_crc8(const uint8_t* data, unsigned int len);
#endif