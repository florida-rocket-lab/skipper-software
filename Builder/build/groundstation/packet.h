#ifndef SKIPPER_PACKET_H
#define SKIPPER_PACKET_H

#include <stddef.h>
#include <stdint.h>
#include "arduino_compat.h"
#include "cobs.h"

// wraps the COBS‐encoded payload in our packet framing:
// [START][LEN][RX_ID][TX_ID][CMD][…PAYLOAD…][CRC]
static inline Pair< UniquePtr<uint8_t[]>, size_t > wrap_packet(
    const uint8_t* encoded_data,
    size_t         encoded_len,
    uint8_t        receiver_id,
    uint8_t        sender_id,
    uint8_t        command_id
) {
    // 1 byte start, 1 byte length, 1 RX, 1 TX, 1 CMD, payload, 1 CRC
    size_t total_len = encoded_len + 6;
    // use make_unique<uint8_t> to get a UniquePtr<uint8_t[]>
    UniquePtr<uint8_t[]> buffer = make_unique<uint8_t>(total_len);

    buffer[0] = 0xAA;                         // start marker
    buffer[1] = static_cast<uint8_t>(total_len);
    buffer[2] = receiver_id;
    buffer[3] = sender_id;
    buffer[4] = command_id;

    // copy the COBS payload
    for (size_t i = 0; i < encoded_len; ++i) {
        buffer[5 + i] = encoded_data[i];
    }

    // CRC8 over the COBS payload
    buffer[total_len - 1] = compute_crc8(encoded_data, encoded_len);

    // Move both the buffer and the length into the Pair<>
    return { Move(buffer), Move(total_len) };
}

#endif // SKIPPER_PACKET_H
