#include <Arduino.h>
#include "nano.h"
#include "_wrapper.h"


Nano::Nano() : _radio(CE_PIN, CSN_PIN), _with_ground(_radio) {} 


void Nano::init_radio(const byte address[6]) {
    _with_ground.init(address);  
    Serial.println("Radio initialized successfully.");

}

void Nano::receive_from_ground() {
    if (_with_ground.receive(_command_data_intermediate)) {
        Serial.println("Command received from Ground!");
        _with_teensy.send_message(_command_data_intermediate);
    }
}

void Nano::send_to_ground() {
    _telemetry_data_intermediate = _with_teensy.receive_message();
    _with_ground.send(_telemetry_data_intermediate);
}

void processCommand(const Ground2Teensy& command) {
    Serial.print("Processing Command: ");
    Serial.println(command.command); 
}

void Nano::listenForPing() {
    if (_with_ground.available()) {
        uint8_t buffer[4] = {0};
        _with_ground.receive(buffer, sizeof(buffer));
        
        if (strncmp((char*)buffer, "PING", 4) == 0) {
            Serial.println("Received PING, sending PONG...");
            uint8_t response[] = "PONG";
            _with_ground.send(response, sizeof(response));
        }
    }
}
