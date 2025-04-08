#ifndef SKIPPER_ARDUINOTEST_H
#define SKIPPER_ARDUINOTEST_H


class ArduinoTest
{
public:
    ArduinoTest(unsigned char fail_pin, unsigned char success_pin);

    void show_assertion(bool assertion);  // This shows a pre-computed assertion; good for when a crash is will CERTAINLY not happen.
    void assert(bool (*assert_function)());  // This shows the "begin-test", and THEN runs the function; good for when a crash may happen.

private:

    void blink_fail();
    void blink_success();
    void blink_both();

    void success();
    void fail();
    void begin();

    unsigned char fail_pin;
    unsigned char success_pin;
};


#endif //SKIPPER_ARDUINOTEST_H
