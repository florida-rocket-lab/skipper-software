
#ifndef SKIPPER_GNC_UNO_H
#define SKIPPER_GNC_UNO_H

#include "../shared/skipper_lib.h"

class Uno
{
public:
    Uno();
    void send_to_sky();
    void receive_from_sky();
    void init_radio();
    Status get_radio_status() const { return nano_pipe.get_status(); }



private:
    UniquePtr<CommandPacket> command_buffer;
    UniquePtr<TelemetryPacket> telemetry_buffer;

    USBCommunication terminal_pipe{};
    RadioCommunication nano_pipe;
};


#endif //SKIPPER_GNC_UNO_H
