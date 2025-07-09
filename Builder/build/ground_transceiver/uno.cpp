#include "uno.h"
#include "constants.h"

// Pull in the ASCII address defined in constants.h ("00001")
const uint8_t Uno::RADIO_ADDRESS[6] = RADIO_COMMUNICATION_ADDRESS;

Uno::Uno()
  : nano_pipe(UNO_CE_PIN, UNO_CSN_PIN, RADIO_ADDRESS, RADIO_COMMUNICATION_CHANNEL)
{}

void Uno::init_radio() {
    nano_pipe.init();
}

void Uno::receive_from_sky() {
    Serial.println("UNO: Checking for incoming telemetry...");

    // Attempt to pull a TelemetryPacket off the air
     telemetry_buffer = nano_pipe.receive<TelemetryPacket>();
    if (!telemetry_buffer) return;

    // UniquePtr::get() returns nullptr if nothing arrived
    if (telemetry_buffer.get()) {
        Serial.println("UNO: Got telemetry!");
        // Print out the XYZ accel and gyro
        Serial.print("  acc → x: ");
        Serial.print(telemetry_buffer->imu_data.acc.x);
        Serial.print("  y: ");
        Serial.print(telemetry_buffer->imu_data.acc.y);
        Serial.print("  z: ");
        Serial.println(telemetry_buffer->imu_data.acc.z);

        Serial.print("  gyr → x: ");
        Serial.print(telemetry_buffer->imu_data.gyr.x);
        Serial.print("  y: ");
        Serial.print(telemetry_buffer->imu_data.gyr.y);
        Serial.print("  z: ");
        Serial.println(telemetry_buffer->imu_data.gyr.z);
    } else {
        Serial.println("UNO: No telemetry received.");
    }
}

// Pull in the template instantiation so the linker sees receive<T>()
template UniquePtr<TelemetryPacket> RadioCommunication::receive<TelemetryPacket>();
