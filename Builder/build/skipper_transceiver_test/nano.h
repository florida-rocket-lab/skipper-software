//
// Created by logan on 3/6/2025.
//

#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include "skipper_lib.h"

class Nano
{
public:
    Nano() = default;
    void receive_from_ground();
    void send_to_ground();

private:
    UniquePtr<CommandPacket> command_buffer;
    UniquePtr<TelemetryPacket> telemetry_buffer;

    RTX1Communication teensy_pipe{};
    RadioCommunication uno_pipe{NANO_CE_PIN, NANO_CSN_PIN};
};


#endif //SKIPPER_GNC_NANO_H
