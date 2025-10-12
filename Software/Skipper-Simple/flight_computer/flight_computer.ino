#include "FlightComputer.h"

FlightComputer fc;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  // Wait up to 3s for USB Serial so we see early prints
  uint32_t t0 = millis();
  while (!Serial && (millis() - t0) < 3000) { }

  Serial.println("Skipper FC boot...");

  if (!fc.begin()) {
    Serial.println("FC begin() FAILED. Check wiring/power/peripherals.");
    // Hard error blink so you see something even without Serial
    for (;;) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(200);
    }
  }

  Serial.println("FC ready.");
}

void loop() {
  fc.update();

  // Tiny heartbeat so you know loop is running even if no telemetry yet
  static uint32_t last = 0;
  if (millis() - last > 500) {
    last = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
