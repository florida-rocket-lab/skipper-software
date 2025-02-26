#include <Arduino.h>
#include "nano.h"

Nano communicator{};

void setup() {
    Serial.begin(9600);
    const byte address[6] = "00001"; 
    communicator.init_radio(address); 
}

void loop() {
    communicator.receive_from_ground();  // Listen for ground commands
    communicator.send_to_ground();       // Send telemetry to ground
}