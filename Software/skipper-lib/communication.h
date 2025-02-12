//
// Created by logan on 2/7/2025.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "datatypes.h"

enum class SkipperFlag : unsigned char
{
  Idle      =     0x00,
  Armed     =     0x01, // These four correspond to the last two bits because
  Takeoff   =     0x02, // they are all mutually exclusive.
  Hover     =     0x03,
  // add more as needed.
};

class UARTConnection
{
public:
  UARTConnection();
  void send_message(const Ground2Teensy &command);
  Teensy2Ground receive_message();
};

class RadioConnection
{
public:
  RadioConnection();
  void send_message(const Teensy2Ground &flight_data);
  Ground2Teensy receive_message();
};

#endif //COMMUNICATION_H
