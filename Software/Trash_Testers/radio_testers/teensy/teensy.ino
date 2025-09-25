/*
 * RF24 Teensy Transmitter → Arduino Uno Receiver
 * Teensy pins below assume CE=37, CSN=36 (change if needed).
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   37
#define CSN_PIN  36
RF24 radio(CE_PIN, CSN_PIN);

const byte ADDRESS[5] = {'T','X','A','A','A'};

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }
  Serial.println(F("=== RF24 Teensy Transmitter ==="));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding! ❌"));
    while (true) { delay(1000); }
  }

  radio.enableDynamicPayloads();
  radio.setAutoAck(true);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);      
  radio.setRetries(5, 15);           
  radio.setAutoAck(true);

  radio.openWritingPipe(ADDRESS);
  radio.stopListening();

  radio.printPrettyDetails();
  Serial.println(F("Transmitter ready ✅"));
}

void loop() {
  static uint32_t counter = 0;
  char payload[32];
  snprintf(payload, sizeof(payload), "HELLO_UNO #%lu", (unsigned long)counter);

  bool ok = radio.write(payload, strlen(payload) + 1);  // include null

  Serial.print(F("TX #"));
  Serial.print(counter++);
  Serial.println(ok ? F(" sent ✅") : F(" failed ❌"));

  delay(200);
}
