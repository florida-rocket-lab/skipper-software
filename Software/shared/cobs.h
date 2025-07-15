#pragma once
#include <stddef.h>
#include <stdint.h>

// COBS encode: returns encoded length, or 0 on overflow
inline size_t cobs_encode(const uint8_t* in, size_t len, uint8_t* out, size_t maxOut)
{
    size_t wr = 1, codePos = 0;
    uint8_t code = 1;
    for (size_t rd = 0; rd < len; ++rd) {
        if (in[rd] == 0) {
            out[codePos] = code;
            code = 1;
            codePos = wr++;
            if (wr >= maxOut) return 0;
        } else {
            out[wr++] = in[rd];
            if (++code == 0xFF) {
                out[codePos] = code;
                code = 1;
                codePos = wr++;
                if (wr >= maxOut) return 0;
            }
        }
    }
    out[codePos] = code;
    return wr;
}

// COBS decode: returns decoded length, or 0 on overflow
inline size_t cobs_decode(const uint8_t* in, size_t len, uint8_t* out, size_t maxOut)
{
    size_t rd = 0, wr = 0;
    while (rd < len) {
        uint8_t code = in[rd++];
        for (uint8_t i = 1; i < code && rd < len; ++i) {
            if (wr >= maxOut) return 0;
            out[wr++] = in[rd++];
        }
        if (code != 0xFF && rd < len) {
            if (wr >= maxOut) return 0;
            out[wr++] = 0;
        }
    }
    return wr;
}

// CRC‑8 (polynomial 0x07)
inline uint8_t compute_crc8(const uint8_t* d, size_t len)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= d[i];
        for (uint8_t b = 0; b < 8; ++b) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }
    }
    return crc;
}

