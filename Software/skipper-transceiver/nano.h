#ifndef NANO_H
#define NANO_H

#include <Arduino.h>
#include "../skipper-lib/UARTConnection.h"
#include "../skipper-lib/RadioConnection.h"
#include "../skipper-lib/Ground2Teensy.h"

class Nano {
  public:
    Nano();
    void init();                // initializes UART & RF
    void sendCommand(uint8_t command);  
    void receiveCommand();    
    void checkConnection();    

  private:
    UARTConnection _with_teensy;
    RadioConnection _with_ground;
};

#endif 