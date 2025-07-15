#include "nano.h"

Nano nano;

void setup() {
  Serial.begin(115200);
  nano.init_radio();         // calls uno_pipe.init() internally
  nano.init_teensy_link();
}

void loop() {
  nano.process_ground_to_teensy();   // pull commands off RF → UART
  nano.process_teensy_to_ground();   // pull IMU off UART → RF
}
