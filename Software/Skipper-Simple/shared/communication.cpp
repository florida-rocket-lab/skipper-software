#include <communication.h>
#include <cobs.h>

bool rf_sendIMU(RF24& radio, const IMUFrame& frame) {
    uint8_t buf[IMU_PACKET_SIZE];
    buf[0] = IMU_START;
    buf[1] = IMU_FRAME_LEN;
    memcpy(buf + 2, &frame, sizeof(IMUFrame));
    buf[IMU_PACKET_SIZE - 1] = compute_crc8(buf + 2, sizeof(IMUFrame));
    return radio.write(buf, IMU_PACKET_SIZE, true);
}

bool rf_recvIMU(RF24& radio, IMUFrame& frame) {
    if (!radio.available()) return false;
    uint8_t raw[IMU_PACKET_SIZE];
    radio.read(raw, IMU_PACKET_SIZE);
    if (raw[0] != IMU_START || raw[1] != IMU_FRAME_LEN) return false;
    if (compute_crc8(raw + 2, sizeof(IMUFrame)) != raw[IMU_PACKET_SIZE - 1]) return false;
    memcpy(&frame, raw + 2, sizeof(IMUFrame));
    return true;
}

bool uart_sendCOBS(Stream& s, const uint8_t* buf, size_t n) {
    uint8_t cobsBuf[MAX_PACKET_SIZE];
    size_t encLen = cobs_encode(buf, n, cobsBuf, sizeof(cobsBuf));
    if (!encLen) return false;
    cobsBuf[encLen] = 0;
    return s.write(cobsBuf, encLen + 1) == encLen + 1;
}

size_t uart_recvCOBS(Stream& s, uint8_t* out, size_t max) {
    uint8_t rawBuf[MAX_PACKET_SIZE];
    size_t len = 0;
    unsigned long start = millis();
    while (millis() - start < MESSAGE_TIMEOUT_MS) {
        if (s.available()) {
            rawBuf[len++] = s.read();
            if (rawBuf[len - 1] == 0) {
                return cobs_decode(rawBuf, len - 1, out, max);
            }
            if (len >= sizeof(rawBuf)) return 0;
        }
    }
    return 0;
}

bool uart_sendIMUFrame(Stream& s, const IMUFrame& frame) {
    uint8_t buf[IMU_FRAME_LEN + 2];
    buf[0] = IMU_START;
    buf[1] = IMU_FRAME_LEN;
    memcpy(buf + 2, &frame, sizeof(IMUFrame));
    buf[IMU_FRAME_LEN + 1] = compute_crc8(buf + 2, sizeof(IMUFrame));
    return s.write(buf, sizeof(buf)) == sizeof(buf);
}

uint8_t compute_crc8(const uint8_t* data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}