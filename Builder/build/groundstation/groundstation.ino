#include <SPI.h>
#include <RF24.h>
#include <stdio.h>
#include "constants.h"  


static int uart_putchar(char c, FILE*) {
  Serial.write(c);
  return c;
}
static FILE uartout;

RF24 radio(UNO_CE_PIN, UNO_CSN_PIN);
const uint8_t address[6] = RADIO_COMMUNICATION_ADDRESS;

void setup() {
  Serial.begin(9600);
  delay(100);

  fdev_setup_stream(&uartout, uart_putchar, nullptr, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  if (!radio.begin()) {
    Serial.println(F("radio.begin() failed"));
    while (1);
  }

  radio.setChannel(76);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(address);
  radio.stopListening();

  radio.printDetails();
  Serial.println(F("successful"));
}

void loop() {
  const char cmd[] = "CMD1";
  bool ok = radio.write(cmd, sizeof(cmd));

  Serial.print(F("Sent -> "));
  if (ok) {
    Serial.println(cmd);
  } else {
    Serial.println(F("FAILED"));
  }

  delay(200);
}
