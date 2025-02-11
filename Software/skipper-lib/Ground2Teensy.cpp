#include "Ground2Teensy.h"

// build the command packet with CRC
void Ground2Teensy::buildPacket(uint8_t command) {
    _command = command;
    _crc = calculateCRC(command);
}

// serialize into a byte array
void Ground2Teensy::serialize(uint8_t* buffer) {
    buffer[0] = START_BYTE;
    buffer[1] = _command;
    buffer[2] = _crc;
    buffer[3] = END_BYTE;
}

// deserialize a received packet
bool Ground2Teensy::deserialize(uint8_t* buffer, uint8_t& command) {
    if (buffer[0] != START_BYTE || buffer[3] != END_BYTE) {
        return false;  // invalid packet structure
    }

    uint8_t receivedCommand = buffer[1];
    uint8_t receivedCRC = buffer[2];

    if (calculateCRC(receivedCommand) != receivedCRC) {
        return false;  // corrupted packet
    }

    command = receivedCommand;
    return true;
}

// simple CRC Calculation
uint8_t Ground2Teensy::calculateCRC(uint8_t command) {
    return command ^ 0xA5;  //bitwise operation with 0xA5, (command ^ 0xA5) ^ 0xA5 = command

}
