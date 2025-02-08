#include <Arduino.h>
#include "ground.h"

Ground::Ground()
{
    // TODO: Initialize USB Serialization with Terminal
    // TODO: Initialize internal variables
}

void Ground::terminal_to_sky() const
{
    // TODO: Read USB Serial
    // TODO: Send terminal_to_sky struct via RadioConnection
}
void Ground::sky_to_terminal() const
{
    // TODO: Wait for sky_to_terminal struct via RadioConnection
    // TODO: Send Data to USB Serial
}
void Ground::communication_loop() const
{
    this->terminal_to_sky();
    this->sky_to_terminal();
}
