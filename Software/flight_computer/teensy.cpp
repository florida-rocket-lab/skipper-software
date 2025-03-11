
#include "teensy.h"

void Teensy::receive_command()
{
    this->command_buffer = this->nano_pipe.receive<CommandPacket>();
}
void Teensy::send_telemetry()
{
    this->nano_pipe.send(this->telemetry_buffer);
}

