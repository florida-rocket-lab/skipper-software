#include "uno.h"

// Only put setup and loop code in the .ino files; any control flow should happen in the .cpp and .h files.
Uno me{};
void setup()
{

}
void loop()
{
    me.receive_from_sky();
    me.send_to_sky();
}
