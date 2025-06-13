#include "nano.h"

const uint8_t Nano::RADIO_ADDRESS[6] = "DownL"; 
Nano::Nano() 
  : uno_pipe(NANO_CE_PIN, NANO_CSN_PIN, RADIO_ADDRESS) {

  }

void Nano::init_radio() {
    uno_pipe.init();
}



void Nano::receive_from_ground() {
    command_buffer = uno_pipe.receive<CommandPacket>();
    if (command_buffer.get()) {
        Serial.print("NANO: Got command -> ");
        Serial.println(command_buffer->message.data);
    }
}

void Nano::send_to_ground() {
    Serial.println("NANO: Sending telemetry...");
    IMUData imu;
    imu.set_acc(1.23, 4.56, 7.89);
    imu.set_gyr(0.12, 0.34, 0.56);

    TelemetryPacket telem;

    telem.imu_data.set_acc(imu.acc.x, imu.acc.y, imu.acc.z);
    telem.imu_data.set_gyr(imu.gyr.x, imu.gyr.y, imu.gyr.z);

    Serial.println("NANO: Sending dummy telemetry to Uno...");
    uno_pipe.send(
        UniquePtr<BaseSerializable>(new TelemetryPacket(telem)), GROUND_STATION_ID, FLIGHT_COMPUTER_ID, CMD_TELEMETRY );
}
