#ifndef GROUND2TEENSY_H
#define GROUND2TEENSY_H

#include <Arduino.h>
#define START_BYTE 0x7E   // '~'
#define END_BYTE   0x81   // '}'



class Ground2Teensy {
  public:
    Ground2Teensy() = default;
    
    // build a command packet with CRC
    void buildPacket(uint8_t command);
    
    // serialize into a byte array
    void serialize(uint8_t* buffer);

    // deserialize received data into a command
    static bool deserialize(uint8_t* buffer, uint8_t& command);
    
  private:
    uint8_t _command;
    uint8_t _crc;

    // CRC Checksum Calculation
    uint8_t calculateCRC(uint8_t command);
};

#endif 