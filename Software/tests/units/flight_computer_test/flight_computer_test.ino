
#include "../../shared/arduinotest.h"
#include "../../../flight_computer/teensy.h"
#include "../../../shared/skipper_lib.h"

ArduinoTest tester{TEENSY_FAIL_PIN, TEENSY_SUCCESS_PIN};
Teensy me{};
void setup()
{

}
void loop()
{
    tester.show_assertion(true);
    tester.show_assertion(false);
}
