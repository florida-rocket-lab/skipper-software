#include <SPI.h>
#include <RF24.h>
#include <stdio.h>
#include "constants.h"


static int uart_putchar(char c, FILE*) {
  Serial.write(c);
  return c;
}
static FILE uartout;  

RF24 radio(NANO_CE_PIN, NANO_CSN_PIN);
const uint8_t address[6] = RADIO_COMMUNICATION_ADDRESS;

void setup() {
  Serial.begin(9600);
  while (!Serial);


  fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  if (!radio.begin()) {
    Serial.println("radio.begin() failed");
    while (1);
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.enableDynamicPayloads();

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println(F("Nano Tester: RADIO listening"));
  radio.printDetails();          
  Serial.println(F("<< dump complete >>"));
}

void loop() {
  char buf[32] = {0};
  while (radio.available()) {
    radio.read(&buf, sizeof(buf));
    Serial.print("Received -> ");
    Serial.println(buf);
  }
}
