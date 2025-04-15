
#include "nano.h"

void Nano::receive_from_ground()
{
    command_buffer = uno_pipe.receive<CommandPacket>();
    if (command_buffer.get()) {
        Serial.print("NANO: Received command -> ");
        Serial.println(command_buffer->message.data); // print the "HELLO_NANO"
    }
}

void Nano::send_to_ground()
{
    TelemetryPacket tp;
    tp.imu_data.set_acc(1.0, 2.0, 3.0);
    tp.imu_data.set_gyr(0.1, 0.2, 0.3);

    Serial.println("NANO: Sending dummy telemetry to Uno...");
    uno_pipe.send(MakeUnique<TelemetryPacket>(new TelemetryPacket(tp)),
                  GROUND_STATION_ID, PAYLOAD_CONTROLLER_ID, CMD_TELEMETRY);
}