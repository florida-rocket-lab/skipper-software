#ifndef SKIPPER_PACKET_H
#define SKIPPER_PACKET_H

#include <stddef.h>
#include <stdint.h>
#include "arduino_compat.h"   // provides Pair<...>
#include "cobs.h"

inline auto wrap_packet(const uint8_t *payload,
                        size_t payload_len,
                        uint8_t rx, uint8_t tx, uint8_t cmd)
        -> Pair<UniquePtr<uint8_t[]>, size_t>     // <— use Pair, not std::pair
{
    const uint16_t len_no_crc = payload_len + 6;   // header + payload
    size_t         total_len  = len_no_crc + 1;    // + CRC

    auto buf = UniquePtr<uint8_t[]>(new uint8_t[total_len]);

    buf[0] = 0xAA;
    buf[1] = len_no_crc & 0xFF;
    buf[2] = len_no_crc >> 8;
    buf[3] = rx;
    buf[4] = tx;
    buf[5] = cmd;

    memcpy(buf.get() + 6, payload, payload_len);

    buf[total_len - 1] =
        compute_crc8(reinterpret_cast<const char*>(payload), payload_len);

    return { Move(buf), Move(total_len) };
}

#endif // SKIPPER_PACKET_H
