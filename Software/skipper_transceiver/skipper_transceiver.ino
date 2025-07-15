

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

#include "packet.h"        
#include "communication.h" 
#include "constants.h"     

constexpr uint8_t TEEN_RX = 2;        
constexpr uint8_t TEEN_TX = 3;        

SoftwareSerial teenLink(TEEN_RX, TEEN_TX);

RF24  radio(NANO_CE_PIN, NANO_CSN_PIN);
const uint8_t RF_ADDR[6] = RADIO_COMMUNICATION_ADDRESS;

bool uart_recvIMUFrame(Stream& s, IMUFrame& out)
{
  if (!s.available() || s.peek() != IMU_START) return false;
  s.read();            

  while (!s.available()) {}
  if (s.read() != IMU_FRAME_LEN) return false;

  uint8_t raw[IMU_FRAME_LEN];
  size_t needed = IMU_FRAME_LEN;
  size_t got    = s.readBytes(raw, needed);
  if (got != needed) return false;

  if (crc8(raw, IMU_FRAME_LEN - 1) != raw[IMU_FRAME_LEN - 1]) return false;

  memcpy(&out, raw, sizeof(IMUFrame));
  return true;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}                 

  teenLink.begin(38400);

  radio.begin();
  radio.setChannel(RADIO_COMMUNICATION_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(RF_ADDR);   
  radio.stopListening();             

  Serial.println(F("Nano relay ready"));
}

void loop()
{
  IMUFrame imu;

  /* 1. read a fresh frame from the Teensy */
  if (uart_recvIMUFrame(teenLink, imu))
  {
    /* 2. print to USB Serial Monitor */
    Serial.print(F("A[g] "));
    Serial.print(imu.ax,3); Serial.print(' ');
    Serial.print(imu.ay,3); Serial.print(' ');
    Serial.print(imu.az,3); Serial.print("   G[dps] ");
    Serial.print(imu.gx,3); Serial.print(' ');
    Serial.print(imu.gy,3); Serial.print(' ');
    Serial.println(imu.gz,3);

    /* 3. forward unchanged over RF to the UNO */
    rf_sendIMU(radio, imu);
  }
}
