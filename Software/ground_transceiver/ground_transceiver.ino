#include "uno.h"

// Only put setup and loop code in the .ino files; any control flow should happen in the .cpp and .h files.
void setup()
{
    Serial.begin(9600);
    while (!Serial);
    Serial.println("UNO: Setup done");
}
void loop()
{
    me.receive_from_sky();
    me.send_to_sky();
}
