#include <SoftwareSerial.h>

SoftwareSerial link(2, 3);

void setup() {
  Serial.begin(9600);    
  link.begin(9600);     
  Serial.println("NANO: ready, listening on D2…");
}

void loop() {
  if (link.available()) {
    int v = link.parseInt();        
    Serial.print("RX from Teensy: ");
    Serial.println(v);
  }
}
