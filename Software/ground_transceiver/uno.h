
#ifndef SKIPPER_GNC_UNO_H
#define SKIPPER_GNC_UNO_H

#include "../shared/skipper_lib.h"

class Uno
{
public:
    Uno() = default;
    void send_to_sky();
    void receive_from_sky();

private:
    UniquePtr<CommandPacket> command_buffer;
    <TelemetryPacket> telemetry_buffer;

    USBCommunication terminal_pipe{};
    RadioCommunication nano_pipe{UNO_CE_PIN, UNO_CSN_PIN};
};


#endif //SKIPPER_GNC_UNO_H
