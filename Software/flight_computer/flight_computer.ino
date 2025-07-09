#include "teensy.h"          
#include "communication.h"
#include "constants.h"

Teensy             board;
RTX3Communication  nano_link;

IMUData imu;                 

void setup() { board.init(); }

void loop() {
  board.read_imu();
  imu = board.getIMUData();

  nano_link.send(&imu,
                 GROUND_STATION_ID,
                 FLIGHT_COMPUTER_ID,
                 CMD_IMU_DATA);      

  delay(50);   
}
