#include "nano.h"

Nano::Nano() {}

void Nano::init() {
    Serial.begin(9600);
    _with_teensy.init(115200);
    _with_ground.init();
    Serial.println("Nano initialized.");
}

void Nano::sendCommand(uint8_t command) {
    Ground2Teensy commandPacket;
    uint8_t buffer[4];

    commandPacket.buildPacket(command);
    commandPacket.serialize(buffer);

    _with_teensy.send(buffer, 4);  // Send over UART
    Serial.println("Command Sent to Teensy.");
}

void Nano::receiveCommand() {
    uint8_t receivedBuffer[4];
    uint8_t command;

    if (_with_teensy.available() >= 4) {
        _with_teensy.receive(receivedBuffer, 4);

        if (Ground2Teensy::deserialize(receivedBuffer, command)) {
            Serial.print("Received Command: ");
            Serial.println(command);
        } else {
            Serial.println("Invalid Command Packet");
        }
    }
}

void Nano::checkConnection() {
    if (!_with_ground.testConnection()) {
        Serial.println("Warning: No RF link to ground.");
    }
    if (!_with_teensy.testConnection()) {
        Serial.println("Warning: No UART link to Teensy.");
    }
}
