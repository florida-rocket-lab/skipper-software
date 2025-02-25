#ifndef RADIOCONNECTION_H
#define RADIOCONNECTION_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_NRF24.h>
#include "communication.h"

class RadioConnection {
  public:
    RadioConnection();
    
    void init(const byte address[6]); // initialization with address parameter
    bool available();  // checks if data is available
    
    void send(const Ground2Teensy& command);
    void send(const Teensy2Ground& flightData);

    bool receive(Ground2Teensy& command);
    bool receive(Teensy2Ground& flightData);
    
    bool testConnection();  

  private:
    RH_NRF24 nrf24;  
};

#endif 
