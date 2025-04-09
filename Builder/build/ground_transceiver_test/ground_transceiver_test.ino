
#include "arduinotest.h"
#include "uno.h"
#include "skipper_lib.h"

ArduinoTest tester{UNO_FAIL_PIN, UNO_SUCCESS_PIN};
Uno me{};
void setup()
{

}
void loop()
{
    tester.show_assertion(true);
    tester.show_assertion(false);
}
