#ifndef RADIOCONNECTION_H
#define RADIOCONNECTION_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_NRF24.h>  

class RadioConnection {
  public:
    RadioConnection();
    void init();                 // Initializes the RF module
    bool available();            // Checks if data is available
    void send(const String& data); 
    bool receive(String& data);  
    bool testConnection();       

  private:
    RH_NRF24 nrf24;  
};

#endif 
