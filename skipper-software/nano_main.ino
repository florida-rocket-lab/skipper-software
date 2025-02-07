#include "teensy.hpp"
#include "nano.hpp"

UARTConnection::UARTConnection()
{
  // TODO: write UART initializer between Nano and Teensy
}
void UARTConnection::send_messsage(const struct Ground2Teensy &command)
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go HI
  // TODO: send a serialized string command to Teensy through UART
}
struct Teensy2Ground UARTConnection::receive_messsage()
{
  // TODO: **WAIT** for a pin connecting Nano and Teensy to go LOW
  // TODO: receive serialized data from Teensy through UART
}

RadioConnection::RadioConnection()
{
  // TODO: write Radio initializer between Teensy and Ground
}
void RadioConnection::send_message(const struct Teensy2Ground &flight_data)
{
  // TODO: send the serialized data from Teensy to Ground through Radio
  // TODO: **WAIT** for the Ground to respond (**MAYBE THROUGH A DIFFERENT FUNCTION LIKE** `RadioConnection::receive_message()`)
}
struct Ground2Teensy RadioConnection::receive_message()
{
  // TODO: **WAIT** for the Ground to respond (**MAYBE THROUGH A DIFFERENT FUNCTION LIKE** `RadioConnection::send_message()`)
  // TODO: receive serialized data from Ground through Radio
}

void Nano::ground_to_teensy()
{
  this->_flight_data_intermediate = this->_with_ground.receive_message();
  this->_with_teensy.send_message(this->_flight_data_intermediate);
}
void Nano::teensy_to_ground()
{
  this->_command_data_intermediate = this->_with_teensy.receive_message();
  this->_with_ground.send_message(this->_command_data_intermediate);
}

Nano communicator{};
void setup()
{
  // I do not know what we need to initialize
}
void loop()
{
  communicator.ground_to_teensy();
  communicator.teensy_to_ground();
}
