
#include "nano.h"

void Nano::receive_from_ground() {
    command_buffer = uno_pipe.receive<CommandPacket>();      // From ground station
    teensy_pipe.send(command_buffer, FLIGHT_COMPUTER_ID, PAYLOAD_CONTROLLER_ID, CMD_COMMAND_PACKET);  // To teensy
}

void Nano::send_to_ground() {
    telemetry_buffer = teensy_pipe.receive<TelemetryPacket>();   // From teensy
    uno_pipe.send(telemetry_buffer, GROUND_STATION_ID, PAYLOAD_CONTROLLER_ID, CMD_TELEMETRY); // To ground
}