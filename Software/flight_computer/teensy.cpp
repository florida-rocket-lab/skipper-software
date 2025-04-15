
#include "teensy.h"

void Teensy::receive_command() {
    command_buffer = nano_pipe.receive<CommandPacket>();  // From Nano
}

void Teensy::send_telemetry() {
    nano_pipe.send(&telemetry_buffer, PAYLOAD_CONTROLLER_ID, FLIGHT_COMPUTER_ID, CMD_TELEMETRY);  // Back to Nano
}