
#ifndef NANO_H
#define NANO_H

#include "teensy.hpp"

class UARTConnection
{
  public:
    UARTConnection();
    void send_message();
    void receive_message();
};

class RadioConnection
{
  public:
    RadioConnection();
    void send_message();
    void receive_message();
};

class Nano
{
  public:
    Nano(): _with_teensy{}, _with_ground{}, _flight_data_intermediate{}, _command_data_intermediate{} {};
    void ground_to_teensy();
    void teensy_to_ground();
  private:
    UARTConnection _with_teensy;
    RadioConnection _with_ground;
    struct Teensy2Ground _flight_data_intermediate;
    struct Ground2Teensy _command_data_intermediate;
};

#endif // NANO_H
