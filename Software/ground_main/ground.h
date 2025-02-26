#ifndef GROUND_H
#define GROUND_H

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include "_wrapper.h"
#include "RadioConnection.h"

// define radio pins
#define CE_PIN 9
#define CSN_PIN 10


class Ground
{
public:
    Ground();

    void communication_loop();  
    void init_radio(const byte address[6]);
    void send_command();
    void receive_telemetry();

private:
    RF24 _radio;
    RadioConnection _with_uno;

    Teensy2Ground _received_flight_data;  
    Ground2Teensy _command_data_buffer;   

};

#endif 
