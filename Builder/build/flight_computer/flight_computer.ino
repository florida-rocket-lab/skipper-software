#include "teensy.h"

Teensy board;

void setup() {
  board.init();
}

void loop() {
  board.read_imu();
  board.printIMUData();   // dump the latest IMU reading
  board.run_gnc();

  delay(100); // or whatever your loop rate is
}
