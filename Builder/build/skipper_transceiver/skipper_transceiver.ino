#include "nano.h"
Nano me;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("NANO: Setup begin");

   me.init_radio();
}

void loop() {
    me.send_to_ground();
    Serial.print("Radio status: ");
    Serial.println((int)me.get_radio_status());
    delay(200);
}
