#include <Arduino.h>
#include "nano.h"
#include "../skipper-lib/_wrapper.h"

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