

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Skipper.h>
#include <printf.h>

RF24 radio(UNO_CE_PIN, UNO_CSN_PIN);

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    radio.begin();
    radio.setRetries(15, 15);
    radio.setAutoAck(false);
    radio.setChannel(RADIO_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(RADIO_ADDRESS);
    radio.openReadingPipe(0, RADIO_ADDRESS);
    radio.disableDynamicPayloads();
    radio.setPayloadSize(IMU_PACKET_SIZE);
    radio.startListening();
    radio.flush_rx();
    printf_begin();
    radio.printDetails();
    delay(1000);
    Serial.println(F("Ground station ready"));
}

void loop() {
    if (radio.available()) {
        uint8_t raw[IMU_PACKET_SIZE];
        radio.read(raw, IMU_PACKET_SIZE);
        Serial.print(F("Uno RX: "));
        for (int i = 0; i < IMU_PACKET_SIZE; i++) {
            Serial.print(raw[i], HEX);
            Serial.print(' ');
        }
        Serial.println();
    }
    delay(50);
}