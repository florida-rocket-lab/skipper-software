//
// Created by logan on 2/7/2025.
//

#include "communication.h"

SkipperState EKF::filter_data(const IMUData& imu_data)
{
  // TODO: filter an IMU Data struct using an extended Kalman filter.
  return {};
}

UARTConnection::UARTConnection()
{
  // TODO: write UART initializer between Nano and Teensy
}
void UARTConnection::send_message(const Ground2Teensy &command)
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go HI
  // TODO: send a serialized string command to Teensy through UART
}
Teensy2Ground UARTConnection::receive_message()
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go LOW
  // TODO: receive serialized data from Teensy through UART
}

RadioConnection::RadioConnection()
{
  // TODO: write Radio initializer between Teensy and Ground
}
void RadioConnection::send_message(const Teensy2Ground &flight_data)
{
  // TODO: send the serialized data from Teensy to Ground through Radio
  // TODO: **WAIT** for the Ground to respond (**MAYBE THROUGH A DIFFERENT FUNCTION LIKE** `RadioConnection::receive_message()`)
}
Ground2Teensy RadioConnection::receive_message()
{
  // TODO: **WAIT** for the Ground to respond (**MAYBE THROUGH A DIFFERENT FUNCTION LIKE** `RadioConnection::send_message()`)
  // TODO: receive serialized data from Ground through Radio
}
