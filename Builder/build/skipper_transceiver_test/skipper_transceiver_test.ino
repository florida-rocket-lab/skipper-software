#include <Arduino.h>
#include "arduino_compat.h"
#include "constants.h"
#include "cobs.h"
#include "communication.h"
#include "datatypes.h"
#include "packet.h"
#include "skipper_lib.h"
#include "arduinotest.h"

#define ROLE_SENDER

#ifdef ROLE_SENDER
  #define CE_PIN       NANO_CE_PIN
  #define CSN_PIN      NANO_CSN_PIN
  #define FAIL_LED     NANO_FAIL_PIN
  #define SUCCESS_LED  NANO_SUCCESS_PIN
  #define MY_ID        FLIGHT_COMPUTER_ID
  #define PEER_ID      GROUND_STATION_ID
#elif defined(ROLE_RECEIVER)
  #error "This sketch must be compiled with ROLE_SENDER"
#endif

ArduinoTest        tester(FAIL_LED, SUCCESS_LED);
RadioCommunication radioComm(
  CE_PIN,
  CSN_PIN,
  (const uint8_t*)RADIO_COMMUNICATION_ADDRESS,
  RADIO_COMMUNICATION_CHANNEL
);

void setup() {
  Serial.begin(115200);
  tester.show_assertion(true);

  radioComm.init();
}

void loop() {
#ifdef ROLE_SENDER
  unsigned long start = millis();

  // Example: Vector3 test (use SUITE_A)
  Vector3 tx(SUITE_A);
  radioComm.send(&tx, PEER_ID, MY_ID, CMD_IMU_DATA);

  // wait up to PING_TIMEOUT_MS for the echo
  UniquePtr<Vector3> rxUP;
  while (millis() - start < PING_TIMEOUT_MS) {
    rxUP = radioComm.receive<Vector3>();
    if (rxUP.get()) break;   // check raw pointer, not bool
  }

  // blink according to whether we got a matching echo
  if (rxUP.get()) {
    Vector3 const& r = *rxUP;
    bool match = (r.x == tx.x && r.y == tx.y && r.z == tx.z);
    tester.show_assertion(match);    // green if true, red if false
    if (match) {
      // on success, halt
      while (true) delay(1000);
    }
  } else {
    // timed out without ever seeing an echo
    tester.show_assertion(false);
  }

  delay(500);
#endif
}
