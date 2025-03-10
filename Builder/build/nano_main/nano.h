
#ifndef NANO_H
#define NANO_H

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include "_wrapper.h"
#include "RadioConnection.h"


//defined pins 
#define CE_PIN 9
#define CSN_PIN 8

class Nano
{
  public:
    Nano();

    void init_radio(const byte address[6]);
    void receive_from_ground();
    void send_to_ground();
    void listenForPing();
    
    
    
  private:
    RF24 _radio;
    UARTConnection _with_teensy;
    RadioConnection _with_ground;

    Ground2Teensy _command_data_intermediate;
    Teensy2Ground _telemetry_data_intermediate;
};

#endif // NANO_H
