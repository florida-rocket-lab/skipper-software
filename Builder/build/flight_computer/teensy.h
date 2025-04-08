
#ifndef SKIPPER_GNC_TEENSY_H
#define SKIPPER_GNC_TEENSY_H

#include "skipper_lib.h"

class Teensy
{
public:
    Teensy() = default;
    void receive_command();
    void send_telemetry();

private:
    UniquePtr<CommandPacket> command_buffer;
    <TelemetryPacket> telemetry_buffer;
    RTX3Communication nano_pipe{};
};


#endif //SKIPPER_GNC_TEENSY_H
