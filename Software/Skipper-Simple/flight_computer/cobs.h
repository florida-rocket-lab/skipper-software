#ifndef COBS_H
#define COBS_H

#include <Arduino.h>

inline size_t cobs_encode(const uint8_t* input, size_t length, uint8_t* output, size_t max_len) {
    size_t read_idx = 0;
    size_t write_idx = 1;
    size_t code_idx = 0;
    uint8_t code = 1;

    if (!input || !output || max_len < length + 2) return 0;

    while (read_idx < length) {
        if (write_idx >= max_len) return 0;
        if (input[read_idx] == 0) {
            output[code_idx] = code;
            code = 1;
            code_idx = write_idx++;
            read_idx++;
        } else {
            output[write_idx++] = input[read_idx++];
            code++;
            if (code == 0xFF) {
                output[code_idx] = code;
                code = 1;
                code_idx = write_idx++;
            }
        }
    }
    output[code_idx] = code;
    return write_idx;
}

inline size_t cobs_decode(const uint8_t* input, size_t length, uint8_t* output, size_t max_len) {
    size_t read_idx = 0;
    size_t write_idx = 0;
    uint8_t code = 0;
    uint8_t i;

    if (!input || !output || length < 1) return 0;

    while (read_idx < length) {
        code = input[read_idx];
        if (read_idx + code > length && code != 1) return 0;
        read_idx++;
        for (i = 1; i < code && read_idx < length; i++) {
            if (write_idx >= max_len) return 0;
            output[write_idx++] = input[read_idx++];
        }
        if (code != 0xFF && read_idx != length) {
            if (write_idx >= max_len) return 0;
            output[write_idx++] = 0;
        }
    }
    return write_idx;
}

#endif