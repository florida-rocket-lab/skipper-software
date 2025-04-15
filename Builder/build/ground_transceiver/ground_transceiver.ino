#include "uno.h"

// Only put setup and loop code in the .ino files; any control flow should happen in the .cpp and .h files.
Uno me;
void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("UNO: Setup done");
    me.init_radio();
  }
  
  void loop() {
    me.receive_from_sky();
    Serial.print("UNO: Radio status: ");
    Serial.println((int)me.get_radio_status());
  }
  