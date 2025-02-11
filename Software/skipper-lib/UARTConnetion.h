#ifndef UARTCONNECTION_H
#define UARTCONNECTION_H

#include <Arduino.h>

class UARTConnection {
  public:
    UARTConnection() = default;
    void init(long baudrate);
    bool available();
    void send(const String& data);
    bool receive(String& data);
    bool testConnection();
  
  private:
    HardwareSerial* serialPort;
};

#endif 

