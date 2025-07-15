#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "link_frame.h"

constexpr uint8_t CE_PIN  = 9;
constexpr uint8_t CSN_PIN = 8;
constexpr uint8_t CHANNEL = 76;
const    uint8_t ADDRESS[6] = "00001";

RF24 radio(CE_PIN, CSN_PIN);
IMUFrame imu;                                  

void setup() {
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println(F("RF24 init failed — check wiring."));
    while (true) {}
  }
  radio.setChannel(CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();              
  radio.openReadingPipe(1, ADDRESS);
  radio.startListening();

  Serial.println(F("GS ready (dynamic payloads)…"));
}

void loop() {
  if (!radio.available()) return;

  uint8_t len = radio.getDynamicPayloadSize();
  if (len != sizeof(IMUFrame)) {        // flush unexpected packet size
    radio.flush_rx();
    return;
  }
  radio.read(&imu, len);

  Serial.print(F("Accel [g]: "));
  Serial.print(imu.ax, 3); Serial.print(", ");
  Serial.print(imu.ay, 3); Serial.print(", ");
  Serial.println(imu.az, 3);

  Serial.print(F("Gyro  [dps]: "));
  Serial.print(imu.gx, 3); Serial.print(", ");
  Serial.print(imu.gy, 3); Serial.print(", ");
  Serial.println(imu.gz, 3);

  Serial.println();
}
