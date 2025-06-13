//
// Created by logan on 3/6/2025.
//

#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include "skipper_lib.h"


class Nano
{
public:

    Nano();
    void receive_from_ground();
    void send_to_ground();
    Status get_radio_status() const { return uno_pipe.get_status(); }
    void init_radio(); 


private:
    static const uint8_t RADIO_ADDRESS[6]; 
    


    UniquePtr<CommandPacket> command_buffer;
    UniquePtr<TelemetryPacket> telemetry_buffer;

    RTX1Communication teensy_pipe{};
    RadioCommunication uno_pipe;


};
#endif //SKIPPER_GNC_NANO_H
