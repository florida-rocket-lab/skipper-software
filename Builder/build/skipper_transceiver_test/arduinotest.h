#ifndef SKIPPER_ARDUINOTEST_H
#define SKIPPER_ARDUINOTEST_H

#include <Arduino.h>
#include "cases.h"

/**
 * @brief A simple test harness for Arduino using LED blink patterns to indicate pass/fail.
 *
 * This class wraps a test assertion and provides visual feedback on two LEDs:
 * - success_pin: blink pattern indicates a passing test
 * - fail_pin: blink pattern indicates a failing test
 *
 * Usage:
 *   ArduinoTest tester(FAIL_LED_PIN, PASS_LED_PIN);
 *   tester.show_assertion(true); // immediate pass indication
 *   tester.assert(myTestFunction); // runs myTestFunction and indicates pass/fail
 */
class ArduinoTest {
public:
    /**
     * @param fail_pin    Digital output pin for the failure LED
     * @param success_pin Digital output pin for the success LED
     */
    ArduinoTest(uint8_t fail_pin, uint8_t success_pin);

        void begin();


    /**
     * @brief Display a precomputed assertion result.
     * @param assertion  True for success, false for failure
     */
    void show_assertion(bool assertion);

    /**
     * @brief Run a test function and display its result.
     * @param assert_function  Pointer to a function returning bool
     */
    void assert(bool (*assert_function)());

private:
    // Blink patterns
    void blink_fail();
    void blink_success();
    void blink_both();

    // High-level states
    void success();
    void fail();

    uint8_t fail_pin;
    uint8_t success_pin;
};

#endif // SKIPPER_ARDUINOTEST_H
