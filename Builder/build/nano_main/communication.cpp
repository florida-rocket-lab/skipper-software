//
// Created by logan on 2/7/2025.
//

#include "communication.h"

UARTConnection::UARTConnection()
{
  // TODO: write UART initializer between Nano and Teensy
}
void UARTConnection::send_message(const Ground2Teensy &command)
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go HI
  // TODO: send a serialized string command to Teensy through UART
}
Teensy2Ground UARTConnection::receive_message()
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go LOW
  // TODO: receive serialized data from Teensy through UART
}

