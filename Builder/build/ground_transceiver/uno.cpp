
#include "uno.h"

void Uno::receive_from_sky()
{
    this->telemetry_buffer = this->nano_pipe.receive<TelemetryPacket>();
    this->terminal_pipe.send(this->telemetry_buffer);
}
void Uno::send_to_sky()
{
    this->command_buffer = this->terminal_pipe.receive<CommandPacket>();
    this->nano_pipe.send(this->command_buffer);
}

