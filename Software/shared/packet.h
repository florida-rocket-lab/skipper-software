#ifndef SKIPPER_PACKET_H
#define SKIPPER_PACKET_H

#include "arduino_compat.h"
#include "cobs.h"

// wraps the encoded data in a strwuctured packet
inline Pair<UniquePtr<char[]>, size_t> wrap_packet(
    const char* encoded_data,
    size_t encoded_len,
    uint8_t receiver_id,
    uint8_t sender_id,
    uint8_t command_id
) {
    size_t total_len = encoded_len + 6;
    auto final_buf = make_unique<char>(total_len);

    final_buf[0] = 0xAA;
    final_buf[1] = total_len;
    final_buf[2] = receiver_id;
    final_buf[3] = sender_id;
    final_buf[4] = command_id;

    for (size_t i = 0; i < encoded_len; ++i)
        final_buf[5 + i] = encoded_data[i];
        
    uint8_t crc = compute_crc8(encoded_data, encoded_len);
    final_buf[total_len - 1] = crc;
    return {Move(final_buf), Move(total_len)};


}

#endif
