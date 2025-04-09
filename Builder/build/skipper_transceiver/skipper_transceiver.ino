#include "nano.h"

// Only put setup and loop code in the .ino files; any control flow should happen in the .cpp and .h files.
Nano me{};
void setup()
{

}
void loop()
{
    me.receive_from_ground();
    me.send_to_ground();
}
