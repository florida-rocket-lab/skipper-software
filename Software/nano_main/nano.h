
#ifndef NANO_H
#define NANO_H

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include "skipper-lib/_wrapper.h"
#include "../skipper-lib/RadioConnection.h"


//defined pins 
#define CE_PIN 9
#define CSN_PIN 8
extern RF24 radio;

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
