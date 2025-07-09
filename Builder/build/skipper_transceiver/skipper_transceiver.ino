#define _SS_MAX_RX_BUFF 256
#include <SoftwareSerial.h>
#include "nano.h"

Nano nano;

void setup() {
  Serial.begin(9600);
  nano.init_radio();
}

void loop() {
  auto link = nano.debug_link();        // SW-Serial from Teensy
  static bool in_frame = false;
  static uint8_t count = 0;

  while (link && link->available()) {
    uint8_t b = link->read();
    if (!in_frame) {
      if (b == 0xAA) {                   // start marker
        in_frame = true;
        count = 0;
        Serial.print("FRAME: ");
        Serial.print("AA ");
      }
    } else if (count < 15) {
      Serial.print(b, HEX);
      Serial.print(' ');
      count++;
    }
    if (count == 15) {
      in_frame = false;
      Serial.println();
    }
  }
}
