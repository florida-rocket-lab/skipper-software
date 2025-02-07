#include "ground.hpp"
#include "teensy.hpp"

Ground::Ground()
{
    // TODO: Initialize USB Serialization with Terminal
    // TODO: Initialize internal variables
}

void Ground::terminal_to_sky()
{
    // TODO: Read USB Serial
    // TODO: Send terminal_to_sky struct via RadioConnection
}
void Ground::sky_to_terminal()
{
    // TODO: Wait for sky_to_terminal struct via RadioConnection
    // TODO: Send Data to USB Serial
}
void Ground::communication_loop()
{
    this->terminal_to_sky();
    this->sky_to_terminal();
}

Ground ground{};
void setup()
{
    // I don't know what needs to be set up
}
void loop()
{
    ground.communication_loop();
}
