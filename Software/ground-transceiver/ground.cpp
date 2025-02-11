#include <Arduino.h>
#include "ground.h"

Ground::Ground()
{
    Serial.begin(115200); 
    _radio.init(); 
}

void Ground::send_command()
{
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');

         if (command == "IDLE") {
            _command_data_buffer.buildPacket(101);
        }else if (command == "ARM") {
            _command_data_buffer.buildPacket(102);
        } else if (command == "TAKEOFF") {
            _command_data_buffer.buildPacket(103);
        } else if (command == "HOVER") {
            _command_data_buffer.buildPacket(104);
        } else if (command == "LAND") {
            _command_data_buffer.buildPacket(105);
        } else {
            Serial.println("Invalid Command!");
            return;
        }

        _radio.send(_command_data_buffer);
        Serial.print("Sent Command: ");
        Serial.println(command);
    }
}

void Ground::receive_telemetry()
{
    if (_radio.receive(_received_flight_data)) {
        Serial.println("Received Telemetry:");
        Serial.print("IMU Acc: "); 
        Serial.print(_received_flight_data.imu_data.acc[0]); Serial.print(", ");
        Serial.print(_received_flight_data.imu_data.acc[1]); Serial.print(", ");
        Serial.println(_received_flight_data.imu_data.acc[2]);

        Serial.print("IMU Gyr: "); 
        Serial.print(_received_flight_data.imu_data.gyr[0]); Serial.print(", ");
        Serial.print(_received_flight_data.imu_data.gyr[1]); Serial.print(", ");
        Serial.println(_received_flight_data.imu_data.gyr[2]);

        Serial.print("Position: ");
        Serial.print(_received_flight_data.skipper_state.pos[0]); Serial.print(", ");
        Serial.print(_received_flight_data.skipper_state.pos[1]); Serial.print(", ");
        Serial.println(_received_flight_data.skipper_state.pos[2]);

        Serial.print("Velocity: ");
        Serial.print(_received_flight_data.skipper_state.vel[0]); Serial.print(", ");
        Serial.print(_received_flight_data.skipper_state.vel[1]); Serial.print(", ");
        Serial.println(_received_flight_data.skipper_state.vel[2]);

        Serial.println("----------------------");
    }
}

void Ground::communication_loop()
{
    send_command();
    receive_telemetry();
}
