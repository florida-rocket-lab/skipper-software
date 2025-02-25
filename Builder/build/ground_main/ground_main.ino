#include "ground.h"

Ground ground;

void setup()
{
    Serial.begin(115200);
    ground.communication_loop();
}

void loop()
{
    ground.communication_loop();  
}
