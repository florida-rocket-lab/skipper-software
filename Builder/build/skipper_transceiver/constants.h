
#ifndef SKIPPER_CONSTANTS_H
#define SKIPPER_CONSTANTS_H

// I've just compiled all the pin information into one spot for easy access. - Logan
#include <Arduino.h>


#define USE_LQI_STUB
inline uint8_t mapThrustToPwm(float thrust) {
  // simple linear mapping 0…maxThrust → 0…255
  constexpr float maxThrust = 1.0f; 
  int v = (int)(thrust / maxThrust * 255.0f);
  return constrain(v, 0, 255);
}

#define RADIO_COMMUNICATION_ADDRESS "00001"
#define RADIO_COMMUNICATION_CHANNEL 76
#define PING_TIMEOUT_MS 500
#define MESSAGE_TIMEOUT_MS 500
#define MESSAGE_SIZE 32

#define NANO_CE_PIN 9   // All the Arduino chips have different CE and CSN pins.
#define NANO_CSN_PIN 8  // I copied these from the great Jose Reynaldo's original
#define UNO_CE_PIN 9    // source code.
#define UNO_CSN_PIN 10
#define MEGA_CE_PIN   9
#define MEGA_CSN_PIN  53

#define BLINK_ON_TIME_MS 200
#define BLINK_OFF_TIME_MS 100

#define UNO_SUCCESS_PIN 5
#define UNO_FAIL_PIN 6
#define NANO_SUCCESS_PIN A1
#define NANO_FAIL_PIN A2
#define TEENSY_SUCCESS_PIN 20
#define TEENSY_FAIL_PIN 21

#define TEENSY_RX3_PIN 15  // Used for Serial3...
#define TEENSY_TX3_PIN 14
#define NANO_RX1_PIN 2    // Used for Serial1...
#define NANO_TX1_PIN 3



// This was found in the test-stand-software.ino; not sure if we are keeping this
#define TEENSY_ESC1_PIN 9     // The original file by Jose says that this controls Servo 1; this is propeller 1 on the contra-rotating propeller
#define TEENSY_ESC2_PIN 7     // Same thing from above applies here, saying that this controls Servo 2; this is the other propeller.
#define TEENSY_BUZZER_PIN 11  // Controls Buzzer


//communication IDs 
#define GROUND_STATION_ID     1
#define FLIGHT_COMPUTER_ID    2
#define FLIGHT_TRANSCEIVER_ID 3

constexpr uint8_t CMD_IMU   = 1;
constexpr uint8_t CMD_ACT   = 2;
constexpr uint8_t CMD_STATE = 3;

// command IDs 
#define CMD_IMU_DATA          101
#define CMD_TELEMETRY         102
#define CMD_COMMAND_PACKET    103
#define CMD_STATE_ESTIMATE    104 



#define MAX_PACKET_SIZE 260


extern uint8_t TX_BUF[MAX_PACKET_SIZE];
extern uint8_t RX_BUF[MAX_PACKET_SIZE];
#endif
