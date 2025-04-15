// ==============================================
// COBS Encoding/Decoding for Packet Framing
// ----------------------------------------------
// This header implements COBS (Consistent Overhead Byte Stuffing), 
// used to encode serialized binary data before transmission.
// COBS removes all 0x00 bytes, making it safe to frame packets 
// with a fixed start byte 0xAA.
//
// How it works (super simple):
// - cobs_encode() replaces 0x00s and adds a code byte every block
// - cobs_decode() reverses the process to get the original data back
//
// Usage:
// 1. After serializing a message but before sending → call `cobs_encode()`
// 2. After receiving raw bytes but before deserializing → call `cobs_decode()`
//
// Typical flow:
//   - serialize() → cobs_encode() → send()
//   - receive() → cobs_decode() → deserialize()
//
// Author: Mr.ChatGPT
// ==============================================
#ifndef SKIPPER_COBS_H
#define SKIPPER_COBS_H

#include "arduino_compat.h"

// max overhead = input size + 1
inline UniquePtr<char[]> cobs_encode(const char* input, size_t length, size_t& out_len) {
    UniquePtr<char[]> output = make_unique<char>(length + 2); // +1 code byte, +1 null terminator
    size_t read_index = 0, write_index = 1, code_index = 0;
    char code = 1;

    while (read_index < length) {
        if (input[read_index] == 0) {
            output[code_index] = code;
            code = 1;
            code_index = write_index++;
            ++read_index;
        } else {
            output[write_index++] = input[read_index++];
            ++code;
            if (code == 0xFF) {
                output[code_index] = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }

    output[code_index] = code;
    out_len = write_index;
    return output;
}

inline UniquePtr<char[]> cobs_decode(const char* input, size_t length, size_t& out_len) {
    UniquePtr<char[]> output = make_unique<char>(length);
    size_t read_index = 0, write_index = 0;

    while (read_index < length) {
        uint8_t code = static_cast<uint8_t>(input[read_index++]);
        for (uint8_t i = 1; i < code; ++i) {
            if (read_index >= length) break;
            output[write_index++] = input[read_index++];
        }
        if (code != 0xFF && read_index < length) {
            output[write_index++] = 0;
        }
    }

    out_len = write_index;
    return output;
}


// need to add more :(

inline uint8_t compute_crc8(const char* data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint8_t>(data[i]);
        for (uint8_t j = 0; j < 8; ++j)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
    }
    return crc;
}


#endif // SKIPPER_COBS_H