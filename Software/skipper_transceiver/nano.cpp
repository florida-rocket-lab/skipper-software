#include "nano.h"
#include "constants.h"
#include "communication.h"
#include <Arduino.h>

const uint8_t Nano::RADIO_ADDRESS[6] = RADIO_COMMUNICATION_ADDRESS;

Nano::Nano()
  : uno_pipe(NANO_CE_PIN, NANO_CSN_PIN, RADIO_ADDRESS, RADIO_COMMUNICATION_CHANNEL)
{}

void Nano::init_teensy_link() {
  teen_link.begin(38400);
  Serial.println("NANO: Teensy link  38 400  on pins 2/3");
}

void Nano::init_radio() {
  uno_pipe.init();
  uno_pipe.start_tx();
  Serial.println("NANO: radio ready (TX)");
}

void Nano::process_ground_to_teensy() {
  uno_pipe.start_rx();
  command_buffer = uno_pipe.receive<CommandPacket>();
  if (command_buffer.get()) {
    Serial.print("NANO: RX cmd → ");
    Serial.println(command_buffer->message.data);
    teensy_pipe.send(
      command_buffer.get(),
      FLIGHT_COMPUTER_ID,
      GROUND_STATION_ID,
      CMD_COMMAND_PACKET
    );
  }
}

void Nano::process_teensy_to_ground() {
    // --- inside Nano::process_teensy_to_ground() ---------------------------
  Stream* s = teensy_pipe.getSerial();
  if (!s) { Serial.println(F("NANO: Serial not init")); return; }

  /* 1. wait for start byte */
  if (!s->available() || s->peek() != 0xAA) return;
  s->read();                               // drop 0xAA

  /* 2. read 5‑byte header */
  uint8_t hdr[5];
  if (s->readBytes(hdr, 5) != 5) return;   // not all here yet
  uint16_t len = uint16_t(hdr[0]) | (uint16_t(hdr[1]) << 8);

  /* 3. payload + CRC */
  size_t toRead = len - 2;                 // payload + 1 CRC
  uint8_t buf[512];                        // plenty big for 256‑B payload
  if (s->readBytes(buf, toRead) != toRead) return;

  /* 4. optional hex dump */
  Serial.print(F("PKT: AA "));
  for (uint8_t b : hdr) { Serial.print(b, HEX); Serial.print(' '); }
  for (size_t i = 0; i < toRead; ++i) { Serial.print(buf[i], HEX); Serial.print(' '); }
  Serial.println();

  /* 5. CRC check */
  size_t pl = toRead - 1;                  // payload length
  if (compute_crc8(reinterpret_cast<char*>(buf), pl) != buf[pl]) {
      Serial.println(F("CRC fail"));
      return;
  }

  /* 6. COBS decode → TelemetryPacket */
  size_t dec_len;
  UniquePtr<char[]> dec = cobs_decode(reinterpret_cast<char*>(buf), pl, dec_len);
  TelemetryPacket tp(Move(dec));           // construct in‑place

  Serial.print(F("Accel: "));
  Serial.print(tp.imu_data.acc.x, 3); Serial.print(", ");
  Serial.print(tp.imu_data.acc.y, 3); Serial.print(", ");
  Serial.println(tp.imu_data.acc.z, 3);

}

// explicit template instantiations…
template UniquePtr<TelemetryPacket> RadioCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket>   UARTCommunication::receive<CommandPacket>();
template UniquePtr<TelemetryPacket> UARTCommunication::receive<TelemetryPacket>();
