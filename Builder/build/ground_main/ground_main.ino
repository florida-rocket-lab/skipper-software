#include "ground.h"

Ground ground;

void setup()
{
    Serial.begin(115200);
    const byte address[6] = "00001"; // Ensure it matches the Nano
    ground.init_radio(address);
    ground.testConnection();

}

void loop()
{
    ground.send_command(); 
}
