
#include "uno.h"
void Uno::receive_from_sky()
{
    telemetry_buffer = nano_pipe.receive<TelemetryPacket>();
    if (telemetry_buffer.get()) {
        Serial.print("UNO: Got telemetry -> acc x: ");
        Serial.println(telemetry_buffer->imu_data.x);
    }
}
void Uno::send_to_sky()
{
    Message<MESSAGE_SIZE> msg;
    strcpy(msg.data, "HELLO_NANO");

    CommandPacket cmd;
    cmd.message = msg;

    Serial.println("UNO: Sending dummy command to Nano...");
    this->nano_pipe.send(MakeUnique<CommandPacket>(new CommandPacket(cmd)),
                         FLIGHT_COMPUTER_ID, GROUND_STATION_ID, CMD_COMMAND_PACKET);
}