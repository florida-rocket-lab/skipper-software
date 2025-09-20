/*
 * Ground Station – RF24 + Serial logger (Arduino Uno)
 * CE → D9   CSN → D10   SCK → D13   MOSI → D11   MISO → D12
 * Matches FlightComputer: RADIO_CHANNEL=80, RADIO_ADDRESS={0,0,0,0,1}
 * Commands: type 'a' (GO/'A'), 'l' (LAND/'L') in Serial Monitor @115200
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>

#include "Skipper.h"       // pins, RADIO_CHANNEL, RADIO_ADDRESS, etc.


// Use the Uno pin defs provided by constants.h
RF24 radio(UNO_CE_PIN, UNO_CSN_PIN);  // CE, CSN

// Optional ACK payload from Flight Computer
#pragma pack(push,1)
struct CmdAck {
  uint8_t tag;   // 0xAC
  uint8_t echo;  // 'A' or 'L'
  uint8_t phase; // FlightPhase enum value
};
#pragma pack(pop)

static const uint8_t CMD_ACK_TAG = 0xAC;

static inline void printCsvHeader() {
  Serial.println(F("t_ms,alt_ft,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,esc_us,servo_x_us,servo_y_us,phase,thrust_milli"));
}

static inline void printTelemetryCSV(const TelemetryV1& t) {
  const float ALT_CM_TO_FT = 0.032808399f;
  const float alt_ft = t.alt_cm * ALT_CM_TO_FT;
  const float ax_g = t.ax_mg / 1000.0f;
  const float ay_g = t.ay_mg / 1000.0f;
  const float az_g = t.az_mg / 1000.0f;
  const float gx_dps = t.gx_dps10 / 10.0f;
  const float gy_dps = t.gy_dps10 / 10.0f;
  const float gz_dps = t.gz_dps10 / 10.0f;

  Serial.print(millis());            Serial.print(',');
  Serial.print(alt_ft, 3);           Serial.print(',');
  Serial.print(ax_g, 3);             Serial.print(',');
  Serial.print(ay_g, 3);             Serial.print(',');
  Serial.print(az_g, 3);             Serial.print(',');
  Serial.print(gx_dps, 3);           Serial.print(',');
  Serial.print(gy_dps, 3);           Serial.print(',');
  Serial.print(gz_dps, 3);           Serial.print(',');
  Serial.print(t.esc_us);            Serial.print(',');
  Serial.print(t.servo_x_us);        Serial.print(',');
  Serial.print(t.servo_y_us);        Serial.print(',');
  Serial.print(t.phase);             Serial.print(',');
  Serial.println(t.thrust_milli);
}

bool sendCommandReliably(char asciiCmd, uint32_t timeout_ms = 2000, uint16_t resend_ms = 50) {
  const uint8_t cmdTX = (asciiCmd == 'a') ? 'A' : 'L';
  uint32_t start   = millis();
  uint32_t lastTX  = 0;
  bool gotAckPayload = false;
  bool gotLinkAck    = false;

  Serial.print(F("Sending command ")); Serial.write(cmdTX); Serial.println(F(" …"));

  while ((uint32_t)(millis() - start) < timeout_ms) {
    // periodic resend
    if ((uint32_t)(millis() - lastTX) >= resend_ms) {
      lastTX = millis();
      radio.stopListening();
      bool ok = radio.write(&cmdTX, 1);   // RAW 1-byte command as FC expects
      radio.startListening();
      gotLinkAck |= ok;                   // link-layer ACK (receiver got it)
    }

    // check for ACK payload (if FC enabled writeAckPayload)
    while (radio.isAckPayloadAvailable()) {
      uint8_t plen = radio.getDynamicPayloadSize();
      if (plen == 0 || plen > 32) plen = 32;
      uint8_t pbuf[32];
      radio.read(pbuf, plen);
      if (plen >= sizeof(CmdAck)) {
        CmdAck ack;
        memcpy(&ack, pbuf, sizeof(CmdAck));
        if (ack.tag == CMD_ACK_TAG && ack.echo == cmdTX) {
          gotAckPayload = true;
          Serial.print(F("ACK payload received (phase="));
          Serial.print(ack.phase);
          Serial.println(F(")"));
          break;
        }
      }
    }
    if (gotAckPayload) break;

    // drain and print telemetry while we wait
    if (radio.available()) {
      TelemetryV1 t;
      if (rf_recvTelemetry(radio, t)) {
        printTelemetryCSV(t);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // blink on RX
      } else {
        // unknown frame: drain it
        uint8_t junk[32];
        uint8_t n = radio.getDynamicPayloadSize();
        if (n == 0 || n > sizeof(junk)) n = sizeof(junk);
        radio.read(junk, n);
      }
    }
  }

  if (gotAckPayload) {
    Serial.println(F("Command confirmed via ACK PAYLOAD ✅"));
    return true;
  } else if (gotLinkAck) {
    Serial.println(F("Command confirmed via link-layer ACK (no ack payload) ✅"));
    return true;
  } else {
    Serial.println(F("Command NOT confirmed (timeout) ❌"));
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {;}

  pinMode(LED_BUILTIN, OUTPUT);

  if (!radio.begin()) {
    Serial.println(F("RF24 init FAIL"));
    while (1) { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); delay(150); }
  }

  radio.setChannel(RADIO_CHANNEL);      // from constants.h
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(5, 15);
  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();              // FC can reply with writeAckPayload()

  // symmetric pipe: matches FC initRadio()
  radio.openReadingPipe(1, RADIO_ADDRESS);
  radio.openWritingPipe(RADIO_ADDRESS);
  radio.startListening();

  printCsvHeader();
  Serial.println(F("GS ready. Type 'a' (GO) or 'l' (LAND). Listening…"));
}

void loop() {
  // handle serial commands
  if (Serial.available()) {
    char c = tolower(Serial.read());
    if (c == 'a' || c == 'l') {
      sendCommandReliably(c);
    }
  }

  // normal telemetry streaming
  if (radio.available()) {
    TelemetryV1 t;
    if (rf_recvTelemetry(radio, t)) {
      printTelemetryCSV(t);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // blink on RX
    } else {
      // drain anything not recognized by our CRC-framed protocol
      uint8_t junk[32];
      uint8_t n = radio.getDynamicPayloadSize();
      if (n == 0 || n > sizeof(junk)) n = sizeof(junk);
      radio.read(junk, n);
    }
  }
}
