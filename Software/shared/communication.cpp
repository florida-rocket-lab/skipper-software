#include "communication.h"
#include <string.h>


// RF helpers (27‑byte IMUFrame burst) 

bool rf_sendIMU(RF24& radio, const IMUFrame& imu)
{
    uint8_t buf[27];
    buf[0] = IMU_START;
    buf[1] = IMU_FRAME_LEN;
    memcpy(buf + 2, &imu, sizeof(IMUFrame));
    buf[26] = crc8(buf + 2, sizeof(IMUFrame));
    return radio.write(buf, sizeof(buf));
}

bool rf_recvIMU(RF24& radio, IMUFrame& imu)
{
    if (!radio.available()) return false;
    uint8_t raw[27];
    radio.read(raw, sizeof(raw));
    if (raw[0] != IMU_START || raw[1] != IMU_FRAME_LEN) return false;
    if (crc8(raw + 2, sizeof(IMUFrame)) != raw[26]) return false;
    memcpy(&imu, raw + 2, sizeof(IMUFrame));
    return true;
}

//= UART helpers (COBS framing)

static uint8_t cobsBuf[260];
static uint8_t rawBuf[260];

bool uart_sendCOBS(Stream& s, const uint8_t* buf, size_t n)
{
    size_t encLen = cobs_encode(buf, n, cobsBuf, sizeof(cobsBuf));
    if (!encLen) return false;
    uint8_t hdr[3] = { 0xAA, uint8_t(encLen+1), uint8_t((encLen+1)>>8) };
    uint8_t crc = compute_crc8(cobsBuf, encLen);
    s.write(hdr, 3);
    s.write(cobsBuf, encLen);
    s.write(crc);
    return true;
}

size_t uart_recvCOBS(Stream& s, uint8_t* out, size_t max)
{
    if (!s.available() || s.peek() != 0xAA) return 0;
    s.read();
    while (s.available() < 2) {}
    uint16_t len = s.read() | (uint16_t(s.read()) << 8);
    if (len < 2 || len > sizeof(rawBuf)) return 0;
    while (s.available() < len) {}
    s.readBytes(rawBuf, len);
    if (compute_crc8(rawBuf, len-1) != rawBuf[len-1]) return 0;
    size_t decLen = cobs_decode(rawBuf, len-1, out, max);
    return decLen;
}


bool uart_sendIMUFrame(Stream& s, const IMUFrame& imu)
{
    uint8_t buf[IMU_FRAME_LEN + 2];        // 0xAA len payload crc
    buf[0] = IMU_START;
    buf[1] = IMU_FRAME_LEN;
    memcpy(buf + 2, &imu, sizeof(IMUFrame));
    buf[IMU_FRAME_LEN + 1] = crc8(buf + 2, sizeof(IMUFrame));
    size_t written = s.write(buf, sizeof(buf));
    return (written == sizeof(buf));
}