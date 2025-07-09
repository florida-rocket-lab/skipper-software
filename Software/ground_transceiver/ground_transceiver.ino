#include "uno.h"

Uno ground;

void setup() {
  Serial.begin(9600);      // for printing the telemetry
  ground.init_radio();     // power up the radio
}

void loop() {
  ground.receive_from_sky();  // check for & print any incoming telemetry
  delay(100);                 // adjust to your desired poll rate
}
