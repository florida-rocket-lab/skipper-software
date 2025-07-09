#include "nano.h"
#include "constants.h"
#include <Arduino.h>

const uint8_t Nano::RADIO_ADDRESS[6] = RADIO_COMMUNICATION_ADDRESS;

Nano::Nano()
  : teensy_pipe(),                          
    uno_pipe(NANO_CE_PIN, NANO_CSN_PIN, RADIO_ADDRESS, RADIO_COMMUNICATION_CHANNEL)
{}

void Nano::init_radio() {
  uno_pipe.init();
  uno_pipe.start_tx();                      
  Serial.println("NANO: radio ready (TX)");
}

void Nano::process_ground_to_teensy() {
  uno_pipe.start_rx();
  command_buffer = uno_pipe.receive<CommandPacket>();
  if (command_buffer.get()) {
    Serial.print("NANO: RX cmd → ");
    Serial.println(command_buffer->message.data);
    teensy_pipe.send(
      command_buffer.get(),
      FLIGHT_COMPUTER_ID,
      GROUND_STATION_ID,
      CMD_COMMAND_PACKET
    );
  }
}

void Nano::process_teensy_to_ground() {
    auto imu = teensy_pipe.receive<IMUData>();  
    if (imu.get()) {                            
        Serial.print("Accel: ");
        Serial.print(imu->acc.x); Serial.print(", ");
        Serial.print(imu->acc.y); Serial.print(", ");
        Serial.println(imu->acc.z);

        uno_pipe.start_tx();
        uno_pipe.send(imu.get(),
                      GROUND_STATION_ID,
                      FLIGHT_COMPUTER_ID,
                      CMD_IMU_DATA);
    }
}



// explicit template instantiations…
template UniquePtr<CommandPacket>   RadioCommunication::receive<CommandPacket>();
template UniquePtr<TelemetryPacket> RadioCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket>   UARTCommunication::receive<CommandPacket>();
template UniquePtr<TelemetryPacket> UARTCommunication::receive<TelemetryPacket>();
