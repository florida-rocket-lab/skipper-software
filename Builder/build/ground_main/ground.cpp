#include <Arduino.h>
#include "ground.h"

Ground::Ground() : _radio(CE_PIN, CSN_PIN), _with_uno(_radio){}



void Ground::init_radio(const byte address[6]) {
    _with_uno.init(address);
    Serial.println("Ground station RF initialized.");
}



void Ground::send_command()
{
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');

        if (command == "IDLE") {
            strcpy(_command_data_buffer.command, "IDLE");
        } else if (command == "ARM") {
            strcpy(_command_data_buffer.command, "ARM");
        } else if (command == "TAKEOFF") {
            strcpy(_command_data_buffer.command, "TAKEOFF");
        } else if (command == "HOVER") {
            strcpy(_command_data_buffer.command, "HOVER");
        } else if (command == "LAND") {
            strcpy(_command_data_buffer.command, "LAND"); // I apologize, but I read from serial in a specific manner; add the header "STDERR: " to specify an error.
        } else {
            Serial.println("STDERR: Invalid Command!");
            return;
        }

        uint8_t buffer[sizeof(Ground2Teensy)];
        memcpy(buffer, &_command_data_buffer, sizeof(Ground2Teensy));
        _radio.write(buffer, sizeof(Ground2Teensy));
        Serial.print("STDOUT: Sent Command: ");
        Serial.print(command);
        Serial.println('\t');  // the "\t" character is replaced with a newline in the code because "\n" is treated as an "end of message" character.
    }
}

void Ground::receive_telemetry()
{
    if (_radio.available()) { 
        uint8_t buffer[sizeof(Teensy2Ground)];
        _radio.read(buffer, sizeof(Teensy2Ground)); 
        memcpy(&_received_flight_data, buffer, sizeof(Teensy2Ground)); 

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
    //receive_telemetry();
}
