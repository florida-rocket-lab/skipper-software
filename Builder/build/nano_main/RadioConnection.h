#ifndef RADIOCONNECTION_H
#define RADIOCONNECTION_H

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "communication.h"


class RadioConnection {
  public:
    RadioConnection(RF24& radioInstance);
    
    void init(const byte address[6]); // initialization with address parameter
    bool available();  // checks if data is available
    
    void send(const Ground2Teensy& command);
    void send(const Teensy2Ground& flightData);
    void send(const uint8_t* buffer, uint8_t len);


    bool receive(Ground2Teensy& command);
    bool receive(Teensy2Ground& flightData);
    bool receive(uint8_t* buffer, uint8_t len);

    
    bool testConnection();  

  private:
    RF24& nrf24;
};

#endif 
