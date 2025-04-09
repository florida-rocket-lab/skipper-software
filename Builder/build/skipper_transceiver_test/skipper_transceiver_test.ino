
#include "arduinotest.h"
#include "nano.h"
#include "skipper_lib.h"

ArduinoTest tester{NANO_FAIL_PIN, NANO_SUCCESS_PIN};
Nano me{};
void setup()
{

}
void loop()
{
    tester.show_assertion(true);
    tester.show_assertion(false);
}
