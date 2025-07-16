#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Skipper.h>            
#include "communication.h"      

static const uint8_t TEENSY_RX = 2;     
static const uint8_t TEENSY_TX = 3; 

class Nano {
public:
  Nano();
  void init_radio();
  void init_teensy_link();  
  void process_ground_to_teensy();
  void process_teensy_to_ground();

  // if you still need it:
  SoftwareSerial* debug_link() {
    return static_cast<SoftwareSerial*>(teensy_pipe.getSerial());
  }

private:
  static const uint8_t RADIO_ADDRESS[6];
  UniquePtr<CommandPacket>   command_buffer;
  UniquePtr<TelemetryPacket> telemetry_buffer;
  SoftwareSerial            teen_link {TEENSY_RX, TEENSY_TX};
  UARTCommunication         teensy_pipe { &teen_link };
  RadioCommunication        uno_pipe;
};

#endif // SKIPPER_GNC_NANO_H
