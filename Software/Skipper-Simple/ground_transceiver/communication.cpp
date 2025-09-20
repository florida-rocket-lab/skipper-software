#include "communication.h"
#include "cobs.h"

static inline void rf_begin_tx(RF24& r){ r.stopListening(); }
static inline void rf_end_tx  (RF24& r){ r.startListening(); }

bool rf_sendPacket(RF24& radio, const void* payload, uint8_t len) {
  uint8_t buf[2 + 24 + 1];                 // hdr + payload(<=24) + crc
  buf[0] = PKT_START;
  buf[1] = len;
  memcpy(buf + 2, payload, len);
  buf[2 + len] = compute_crc8((uint8_t*)payload, len);
  rf_begin_tx(radio);
  bool ok = radio.write(buf, 2 + len + 1, true);
  rf_end_tx(radio);
  return ok;
}

bool rf_recvPacket(RF24& radio, uint8_t* payload, uint8_t expect_len) {
  if (!radio.available()) return false;
  uint8_t buf[2 + 24 + 1];
  radio.read(buf, 2 + expect_len + 1);
  if (buf[0] != PKT_START || buf[1] != expect_len) return false;
  uint8_t crc = buf[2 + expect_len];
  memcpy(payload, buf + 2, expect_len);
  return compute_crc8(payload, expect_len) == crc;
}

bool rf_sendTelemetry(RF24& radio, const TelemetryV1& t) {
  return rf_sendPacket(radio, &t, TELEM_FRAME_LEN);
}
bool rf_recvTelemetry(RF24& radio, TelemetryV1& t) {
  return rf_recvPacket(radio, (uint8_t*)&t, TELEM_FRAME_LEN);
}
uint8_t compute_crc8(const uint8_t* data, unsigned int len) {
  uint8_t crc = 0;
  for (unsigned int i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x07) : (uint8_t)(crc << 1);
    }
  }
  return crc;
}