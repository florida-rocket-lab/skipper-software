#ifndef GROUND_H
#define GROUND_H

#include <Arduino.h>
#include "../skipper-lib/_wrapper.h"
#include "../skipper-lib/communication.h"

class Ground
{
public:
    Ground();
    void communication_loop();  /

private:
    RadioConnection _radio; 

    Teensy2Ground _received_flight_data;  
    Ground2Teensy _command_data_buffer;   

    void send_command();  
    void receive_telemetry(); 
};

#endif // GROUND_H
