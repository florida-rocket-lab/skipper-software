#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include "skipper_lib.h"

class Nano {
public:
    Nano();
    void init_radio();
    void process_ground_to_teensy();
    void process_teensy_to_ground();

    /// debugging: raw SoftwareSerial link to Teensy
    SoftwareSerial* debug_link() {
        // use the public getter from UARTCommunication
        return static_cast<SoftwareSerial*>(teensy_pipe.getSerial());
    }

private:
    static const uint8_t RADIO_ADDRESS[6];
    UniquePtr<CommandPacket>   command_buffer;
    UniquePtr<TelemetryPacket> telemetry_buffer;

    /// UART link to Teensy
    RTX1Communication           teensy_pipe;

    /// nRF24 link to UNO
    RadioCommunication          uno_pipe;
};

#endif // SKIPPER_GNC_NANO_H
