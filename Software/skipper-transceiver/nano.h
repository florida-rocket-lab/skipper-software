
#ifndef NANO_H
#define NANO_H

#include <Arduino.h>
#include "../skipper-lib/_wrapper.h"

class Nano
{
  public:
    Nano() = default;
    void ground_to_teensy();
    void teensy_to_ground();
  private:
    UARTConnection _with_teensy;
    RadioConnection _with_ground;
    Ground2Teensy _flight_data_intermediate;
    Teensy2Ground _command_data_intermediate;
};

#endif // NANO_H
