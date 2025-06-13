
#include "uno.h"


static constexpr uint8_t RADIO_ADDRESS[6] = "DownL";
Uno::Uno() : nano_pipe(UNO_CE_PIN, UNO_CSN_PIN, RADIO_ADDRESS) {}
void Uno::init_radio() {
    nano_pipe.init();  
}

void Uno::receive_from_sky()
{
    Serial.println("UNO: Checking for incoming telemetry...");

    telemetry_buffer = nano_pipe.receive<TelemetryPacket>();

    if (telemetry_buffer.get()) {
        Serial.println("UNO: Got telemetry!");
        Serial.print("acc x: ");
        Serial.println(telemetry_buffer->imu_data.acc.x);
    } else {
        Serial.println("UNO: No telemetry received.");
    }
}

void Uno::send_to_sky()
{
    Message<MESSAGE_SIZE> msg;
    strcpy(msg.data, "HELLO_NANO");

    CommandPacket cmd;
    cmd.message = msg;

    Serial.println("UNO: Sending dummy command to Nano...");
    this->nano_pipe.send(UniquePtr<BaseSerializable>(new CommandPacket(cmd)),
                     FLIGHT_COMPUTER_ID, GROUND_STATION_ID, CMD_COMMAND_PACKET);
}