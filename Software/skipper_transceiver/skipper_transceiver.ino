#define _SS_MAX_RX_BUFF 256
#include <SoftwareSerial.h>
#include "nano.h"

Nano nano;

void setup() {
  Serial.begin(9600);
  nano.init_radio();
  Serial.println("NANO: ready");
}

void loop() {
  auto link = nano.debug_link();
  while (link && link->available()) {
    Serial.print(link->read(), HEX);
    Serial.print(' ');
  }

  nano.process_ground_to_teensy();
  nano.process_teensy_to_ground();
}
