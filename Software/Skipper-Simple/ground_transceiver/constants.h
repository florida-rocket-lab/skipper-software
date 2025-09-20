
#ifndef SKIPPER_CONSTANTS_H
#define SKIPPER_CONSTANTS_H
#pragma once
// I've just compiled all the pin information into one spot for easy access. - Logan
#include <Arduino.h>


#define USE_LQI_STUB


constexpr uint8_t RADIO_ADDRESS[5] = {0, 0, 0, 0, 1};
constexpr uint8_t RADIO_CHANNEL = 80;

constexpr uint8_t PKT_START    = 0xAA;
constexpr uint8_t PKT_OVERHEAD = 3; // start + len + crc

constexpr uint8_t IMU_FRAME_LEN    = sizeof(IMUFrame);     // 24
constexpr uint8_t IMU_PACKET_SIZE  = IMU_FRAME_LEN  + PKT_OVERHEAD;

constexpr uint8_t TELEM_FRAME_LEN  = sizeof(TelemetryV1);  // 24
constexpr uint8_t TELEM_PACKET_SIZE= TELEM_FRAME_LEN + PKT_OVERHEAD;


constexpr uint8_t MESSAGE_SIZE    = 32;           // nRF24 max payload
constexpr uint8_t MAX_PACKET_SIZE = MESSAGE_SIZE; 


#define PING_TIMEOUT_MS 500
#define MESSAGE_TIMEOUT_MS 500

#define NANO_CE_PIN 9   // All the Arduino chips have different CE and CSN pins.
#define NANO_CSN_PIN 8  // I copied these from the great Jose Reynaldo's original
#define UNO_CE_PIN 9    // source code.
#define UNO_CSN_PIN 10
#define MEGA_CE_PIN   9
#define MEGA_CSN_PIN  53
#define TEENSY_CE_PIN   37
#define TEENSY_CSN_PIN  36

#define BLINK_ON_TIME_MS 200
#define BLINK_OFF_TIME_MS 100

#define UNO_SUCCESS_PIN 5
#define UNO_FAIL_PIN 6
#define NANO_SUCCESS_PIN 21
#define NANO_FAIL_PIN A2
#define TEENSY_SUCCESS_PIN 31
#define TEENSY_FAIL_PIN 32

#define TEENSY_RX3_PIN 15  // Used for Serial3...
#define TEENSY_TX3_PIN 14
#define NANO_RX1_PIN 2    // Used for Serial1...
#define NANO_TX1_PIN 3



// This was found in the test-stand-software.ino; not sure if we are keeping this
#define TEENSY_ESC1_PIN 8     // The original file by Jose says th   at this controls Servo 1; this is propeller 1 on the contra-rotating propeller
#define TEENSY_ESC2_PIN 7     // Same thing from above applies here, saying that this controls Servo 2; this is the other propeller.
#define TEENSY_BUZZER_PIN 30  // Controls Buzzer

#define TEENSY_UPPER_GIMBAL_PIN 5
#define TEENSY_LOWER_GIMBAL_PIN 6

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




extern uint8_t TX_BUF[MAX_PACKET_SIZE];
extern uint8_t RX_BUF[MAX_PACKET_SIZE];
#endif
