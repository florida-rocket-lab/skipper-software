#include <Arduino.h>
#include "constants.h"
#include "arduinotest.h"

ArduinoTest::ArduinoTest(unsigned char fail_pin, unsigned char success_pin)
    : fail_pin(fail_pin), success_pin(success_pin)
{
    pinMode(this->fail_pin, OUTPUT);
    pinMode(this->success_pin, OUTPUT);
}

void ArduinoTest::blink_fail()
{
    digitalWrite(this->fail_pin, HIGH);
    delay(BLINK_ON_TIME_MS);
    digitalWrite(this->fail_pin, LOW);
    delay(BLINK_OFF_TIME_MS);
}

void ArduinoTest::blink_success()
{
    digitalWrite(this->success_pin, HIGH);
    delay(BLINK_ON_TIME_MS);
    digitalWrite(this->success_pin, LOW);
    delay(BLINK_OFF_TIME_MS);
}

void ArduinoTest::blink_both()
{
    digitalWrite(this->success_pin, HIGH);
    digitalWrite(this->fail_pin, HIGH);
    delay(BLINK_ON_TIME_MS);
    digitalWrite(this->success_pin, LOW);
    digitalWrite(this->fail_pin, LOW);
    delay(BLINK_OFF_TIME_MS);
}

void ArduinoTest::success()
{
    for (char i = 0; i < 3; ++i) {
        blink_success();
    }
}

void ArduinoTest::fail()
{
    for (char i = 0; i < 3; ++i) {
        blink_fail();
    }
}

void ArduinoTest::begin()
{
    for (char i = 0; i < 3; ++i) {
        blink_both();
    }
}

void ArduinoTest::show_assertion(bool assertion)
{
    begin();
    if (assertion) {
        success();
    } else {
        fail();
    }
}

void ArduinoTest::assert(bool (*assert_function)())
{
    begin();
    if (assert_function()) {
        success();
    } else {
        fail();
    }
}
