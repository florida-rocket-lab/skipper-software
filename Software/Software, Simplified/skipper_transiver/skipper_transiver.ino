

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "constants.h"    
#include "link_frame.h"    

SoftwareSerial teen(2, 3);                     
RF24 radio(NANO_CE_PIN, NANO_CSN_PIN);        


bool readIMU(IMUFrame& out)
{
  static uint8_t state = 0, idx = 0, buf[sizeof(IMUFrame)];
  static uint8_t want  = 0;

  while (teen.available()) {
    uint8_t b = teen.read();
    switch (state) {
      case 0:                               // wait for 0xAA
        if (b == START) state = 1;
        break;

      case 1:                               // length byte
        want  = b;
        idx   = 0;
        state = (want == FRAME_LEN) ? 2 : 0;
        break;

      case 2:                               // payload
        buf[idx++] = b;
        if (idx == sizeof(IMUFrame)) state = 3;
        break;

      case 3:                               // CRC
        if (crc8(buf, sizeof(IMUFrame)) == b) {
          memcpy(&out, buf, sizeof(IMUFrame));
          state = 0;
          return true;                      // good frame!
        }
        state = 0;                          // CRC fail then resync
        break;
    }
  }
  return false;
}

void initRadio()
{
  if (!radio.begin()) {
    Serial.println(F("RF24 init failed"));
    while (true) delay(1000);
  }
  radio.enableDynamicPayloads();
  radio.setChannel(RADIO_COMMUNICATION_CHANNEL);   // 76 by default
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(reinterpret_cast<const uint8_t*>(RADIO_COMMUNICATION_ADDRESS));
  radio.stopListening();                           // TX‑only
  Serial.println(F("NANO: radio ready (TX)"));
}

void setup()
{
  Serial.begin(115200);         
  teen.begin(38400);            
  initRadio();

  Serial.println(F("NANO: Teensy link 38 400 baud on pins 2/3"));
}

void loop()
{
  IMUFrame imu;

  if (readIMU(imu)) {
    Serial.print(F("Accel: "));
    Serial.print(imu.ax, 3); Serial.print(',');
    Serial.print(imu.ay, 3); Serial.print(',');
    Serial.println(imu.az, 3);

    bool ok = radio.write(&imu, sizeof(IMUFrame));
    if (!ok) Serial.println(F("RF24: send fail"));
  }
}
