#include <Arduino.h>
#include <Skipper.h>
#include "teensy.h"      // defines class Teensy

// the UART link to the Nano
UARTCommunication transceiver(&Serial3);

// rename the instance so it doesn't shadow the `teensy` namespace
Teensy imuBoard;

void setup() {
  Serial.begin(115200);
  Serial3.begin(38400);  

  while (!Serial) { }
  
  imuBoard.init();                
  Serial.println(F("Flight computer ready"));
}

void loop() {
  imuBoard.read_imu();               // was teensy.read_imu();
  IMUData imu = imuBoard.getIMUData();  // was teensy.getIMUData();
  imuBoard.printIMUData();
  transceiver.send(&imu,
                   FLIGHT_TRANSCEIVER_ID,
                   FLIGHT_COMPUTER_ID,
                   CMD_IMU_DATA);

  Serial.println(F("Sent IMU packet"));
  delay(100);
}
