
#include "nano.h"

void Nano::receive_from_ground()
{
    this->command_buffer = this->uno_pipe.receive<CommandPacket>();
    this->teensy_pipe.send(this->command_buffer);
}
void Nano::send_to_ground()
{
    this->telemetry_buffer = this->teensy_pipe.receive<TelemetryPacket>();
    this->uno_pipe.send(this->telemetry_buffer);
}
