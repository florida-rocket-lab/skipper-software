#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

const byte address[5] = {'T','X','A','A','A'};

void setup() {
  pinMode(10, OUTPUT);                 
  Serial.begin(115200);
  Serial.println(F("=== RF24 Uno Receiver ==="));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (true) { delay(1000); }
  }
radio.enableDynamicPayloads();
radio.setAutoAck(true);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);       
  radio.printPrettyDetails();

  radio.openReadingPipe(1, address);
  radio.startListening();

  Serial.println(F("Receiver ready ✅ (waiting for packets)"));
}

void loop() {
 if (radio.available()) {
  uint8_t n = radio.getDynamicPayloadSize();
  if (n == 0 || n > 32) n = 32;
  uint8_t buf[32]; 
  radio.read(buf, n);

  Serial.print(F("RX(")); Serial.print(n); Serial.print(F("): "));
  for (uint8_t i=0; i<n; ++i) {
    if (buf[i] < 16) Serial.print('0');
    Serial.print(buf[i], HEX); Serial.print(' ');
  }
  Serial.print(F(" | '"));
  for (uint8_t i=0; i<n; ++i) Serial.write(isprint(buf[i]) ? buf[i] : '.');
  Serial.println('\'');
}
}
