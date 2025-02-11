
#include "nano.h"

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
