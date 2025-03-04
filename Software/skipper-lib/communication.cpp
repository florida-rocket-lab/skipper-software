//
// Created by logan on 2/7/2025.
//

#include "communication.h"

UARTConnection::UARTConnection()
{
    Serial1.begin(9600); // Uses TX Pin (D0) to transmit and RX Pin (D1) to receive.
}
void UARTConnection::send_message(const Ground2Teensy &command)
{
    uint8_t* serial_buffer = uint8_t[Ground2Teensy::command_length];
    command.serialize(serial_buffer) // pass by pointer and memcpy'ed
    Serial1.write(command, Ground2Teensy::command_length * sizeof(uint8_t));
    // TODO: VERIFY THIS CODE; HOLY MOLY IT'S GOOFY.
}
Teensy2Ground UARTConnection::receive_message()
{
    uint8_t* serial_buffer = uint8_t[sizeof(Teensy2Ground)];
    while (Serial1.available() != sizeof(Teensy2Ground)) {};  // Check this out; very cursed.
    Serial1.readBytes(serial_buffer, sizeof(Teensy2Ground));
    return {serial_buffer};
    // TODO: VERIFY THIS CODE; HOLY MOLY IT'S GOOFY.
}

