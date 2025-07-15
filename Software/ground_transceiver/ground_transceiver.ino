#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "packet.h"
#include "communication.h"
#include "constants.h"

RF24      radio( UNO_CE_PIN, UNO_CSN_PIN );
const uint8_t RF_ADDR[6] = RADIO_COMMUNICATION_ADDRESS;

IMUFrame  imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  radio.begin();
  radio.setChannel(RADIO_COMMUNICATION_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, RF_ADDR);
  radio.startListening();

  Serial.println("Ground station ready");
}

void loop() {
  if (rf_recvIMU(radio, imu)) {
    Serial.print("Accel [g]: ");
    Serial.print(imu.ax, 3); Serial.print(", ");
    Serial.print(imu.ay, 3); Serial.print(", ");
    Serial.println(imu.az, 3);

    Serial.print("Gyro  [dps]: ");
    Serial.print(imu.gx, 3); Serial.print(", ");
    Serial.print(imu.gy, 3); Serial.print(", ");
    Serial.println(imu.gz, 3);

    Serial.println();
  }
}
