#ifndef SKIPPER_PACKET_H
#define SKIPPER_PACKET_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>         
#include "arduino_compat.h"
#include "cobs.h"


inline auto wrap_packet(const uint8_t *payload,
                        size_t payload_len,
                        uint8_t RX, uint8_t TX, uint8_t CMD)
      -> Pair<UniquePtr<uint8_t[]>, size_t>
{
    uint16_t len_no_crc = payload_len + 3;        // RX + TX + CMD  (3 bytes)
    size_t   total_len  = payload_len + 7;          // start + header(5) + payload + CRC

    auto buf = UniquePtr<uint8_t[]>(new uint8_t[total_len]);

    buf[0] = 0xAA;
    buf[1] = len_no_crc & 0xFF;                     // LEN LSB
    buf[2] = len_no_crc >> 8;                       // LEN MSB
    buf[3] = RX;                                    
    buf[4] = TX;
    buf[5] = CMD;
    memcpy(buf.get() + 6, payload, payload_len);
    buf[6 + payload_len] = compute_crc8(
            reinterpret_cast<const char*>(buf.get() + 6),
            payload_len);

    return { Move(buf), Move(total_len) };
  }
#endif 
