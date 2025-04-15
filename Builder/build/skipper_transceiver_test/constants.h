//
// Created by logan on 3/6/2025.
//

#ifndef SKIPPER_CONSTANTS_H
#define SKIPPER_CONSTANTS_H

// I've just compiled all the pin information into one spot for easy access.
#include <Arduino.h>

#define RADIO_COMMUNICATION_ADDRESS "00001"
#define PING_TIMEOUT_MS 500
#define MESSAGE_TIMEOUT_MS 500
#define MESSAGE_SIZE 32

#define NANO_CE_PIN 9   // All the Arduino chips have different CE and CSN pins.
#define NANO_CSN_PIN 8  // I copied these from the great Jose Reynaldo's original
#define UNO_CE_PIN 9    // source code.
#define UNO_CSN_PIN 10

#define BLINK_ON_TIME_MS 200
#define BLINK_OFF_TIME_MS 100

// TODO: CHANGE THESE PINS, MR. JOSE
#define UNO_SUCCESS_PIN 20
#define UNO_FAIL_PIN 21
#define NANO_SUCCESS_PIN A1
#define NANO_FAIL_PIN A2
#define TEENSY_SUCCESS_PIN 20
#define TEENSY_FAIL_PIN 21

#define TEENSY_RX3_PIN 15  // Used for Serial3...
#define TEENSY_TX3_PIN 14
#define NANO_RX1_PIN 1     // Used for Serial1...
#define NANO_TX1_PIN 0

// This was found in the test-stand-software.ino; not sure if we are keeping this
#define TEENSY_ESC1_PIN 9     // The original file by Jose says that this controls Servo 1; this is propeller 1 on the contra-rotating propeller
#define TEENSY_ESC2_PIN 7     // Same thing from above applies here, saying that this controls Servo 2; this is the other propeller.
#define TEENSY_BUZZER_PIN 11  // Controls Buzzer

#define MAX_PACKET_SIZE 64  // might need to adjust based on largest serialized+encoded message




#endif //SKIPPER_CONSTANTS_H
