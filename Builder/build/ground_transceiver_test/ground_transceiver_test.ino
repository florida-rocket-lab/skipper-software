#include <Arduino.h>
#include "arduino_compat.h"
#include "constants.h"
#include "cobs.h"
#include "communication.h"
#include "datatypes.h"
#include "packet.h"
#include "skipper_lib.h"
#include "arduinotest.h"

// ===========================================================================
//                                  ROLE
// ===========================================================================
// This is your **ground station** side — must be ROLE_RECEIVER:
#define ROLE_RECEIVER

// --- Pin & ID setup based on role  ----------------------------------------
#ifdef ROLE_SENDER
  #error "This sketch must be compiled with ROLE_RECEIVER"
#elif defined(ROLE_RECEIVER)
  #define CE_PIN       UNO_CE_PIN
  #define CSN_PIN      UNO_CSN_PIN
  #define FAIL_LED     UNO_FAIL_PIN
  #define SUCCESS_LED  UNO_SUCCESS_PIN
  #define MY_ID        GROUND_STATION_ID
  #define PEER_ID      FLIGHT_COMPUTER_ID
#endif

// --- Tester & Radio instances  --------------------------------------------
ArduinoTest        tester(FAIL_LED, SUCCESS_LED);
RadioCommunication radioComm(
  CE_PIN,
  CSN_PIN,
  (const uint8_t*)RADIO_COMMUNICATION_ADDRESS,
  RADIO_COMMUNICATION_CHANNEL
);

// ===========================================================================
//                                  “main” sketch
// ===========================================================================
void setup() {
  Serial.begin(115200);
  // 3× both‐LED blink to show “ready for echoes”
  tester.show_assertion(true);

  radioComm.init();
}

void loop() {
  // try to receive exactly one Vector3
  UniquePtr<Vector3> rxUP = radioComm.receive<Vector3>();

  if (rxUP.get()) {
    // echo it right back
    radioComm.send(rxUP.get(), PEER_ID, MY_ID, CMD_IMU_DATA);
    // blink green
    tester.show_assertion(true);
  } else {
    // nothing received this pass: blink red
    tester.show_assertion(false);
  }

  delay(200);
}
