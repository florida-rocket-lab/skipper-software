
#include "nano.h"

Nano communicator{};

Ground2Teensy receivedCommand;

void setup() {
    Serial.begin(9600);
    const byte address[6] = "00001"; // some unique address
    radio.init(address);
    Serial.println("RF Initialized");


}


void loop()
{
  communicator.ground_to_teensy();
  communicator.teensy_to_ground();
}

void loop() {
  // check if a command is available
  if (radio.receive(receivedCommand)) {
      Serial.println("Command received!");

      // Handle the received command
      processCommand(receivedCommand); // is already deserialized 

      // send handshake response back to ground station (not setup yet)
      //radio.send(handshakeResponse);
      //Serial.println("Handshake response sent.");
  }
}


void processCommand(const Ground2Teensy& command) {
  switch (command.command) {
      case 1:
          Serial.println("Command: ARM");
          break;
      case 2:
          Serial.println("Command: TAKEOFF");
          break;
      case 3:
          Serial.println("Command: LAND");
          break;
      default:
          Serial.println("Unknown command.");
          break;
  }
}
