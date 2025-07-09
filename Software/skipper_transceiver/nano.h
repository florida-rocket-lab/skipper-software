#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include "../shared/skipper_lib.h"

class Nano {
public:
    Nano();
    void init_radio();
    void process_ground_to_teensy();
    void process_teensy_to_ground();

    // debuging which dosen't wor 
    SoftwareSerial* debug_link() {
        return static_cast<SoftwareSerial*>(teensy_pipe.getSerial());
    }

private:
    static const uint8_t RADIO_ADDRESS[6];
    UniquePtr<CommandPacket>   command_buffer;
    UniquePtr<TelemetryPacket> telemetry_buffer;

    RTX1Communication           teensy_pipe;

    RadioCommunication          uno_pipe;
};

#endif
