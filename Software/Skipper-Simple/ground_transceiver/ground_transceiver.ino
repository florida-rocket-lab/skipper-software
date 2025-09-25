/*
 * Ground Station – RF24 + Serial logger (Arduino Uno)
 * CE→UNO_CE_PIN  CSN→UNO_CSN_PIN  (from Skipper.h)
 * Type 'a' (GO) or 'l' (LAND) at 115200.
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include "Skipper.h"          // UNO_CE_PIN, UNO_CSN_PIN, RADIO_CHANNEL, RADIO_ADDRESS, TelemetryV1, rf_recvTelemetry()

RF24 radio(UNO_CE_PIN, UNO_CSN_PIN);

#pragma pack(push,1)
struct CmdAck { uint8_t tag, echo, phase; };
#pragma pack(pop)
static const uint8_t CMD_ACK_TAG = 0xAC;

static inline void printCsvHeader() {
  Serial.println(F("t_ms,alt_ft,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,esc_us,servo_x_us,servo_y_us,phase,thrust_milli"));
}
static inline void printTelemetryCSV(const TelemetryV1& t) {
  const float FT = 0.032808399f;
  Serial.print(millis());            Serial.print(',');
  Serial.print(t.alt_cm * FT, 3);    Serial.print(',');
  Serial.print(t.ax_mg / 1000.0f,3); Serial.print(',');
  Serial.print(t.ay_mg / 1000.0f,3); Serial.print(',');
  Serial.print(t.az_mg / 1000.0f,3); Serial.print(',');
  Serial.print(t.gx_dps10 / 10.0f,3);Serial.print(',');
  Serial.print(t.gy_dps10 / 10.0f,3);Serial.print(',');
  Serial.print(t.gz_dps10 / 10.0f,3);Serial.print(',');
  Serial.print(t.esc_us);            Serial.print(',');
  Serial.print(t.servo_x_us);        Serial.print(',');
  Serial.print(t.servo_y_us);        Serial.print(',');
  Serial.print(t.phase);             Serial.print(',');
  Serial.println(t.thrust_milli);
}

/* ---------- Command sender (non-blocking state machine) ---------- */
static bool     cmd_pending = false;
static uint8_t  cmdTX = 0;
static uint32_t cmd_start_ms = 0, last_tx_ms = 0;
static const uint16_t RESEND_MS = 50;

void beginCommand(char asciiCmd) {
  cmdTX = (asciiCmd == 'a') ? 'A' : 'L';
  cmd_pending = true;
  cmd_start_ms = millis();
  last_tx_ms = 0;
  Serial.print(F("CMD ")); Serial.write(cmdTX); Serial.println(F("…"));
}

void finishCommand(const __FlashStringHelper* why) {
  Serial.print(F("CMD ")); Serial.write(cmdTX); Serial.print(F(" "));
  Serial.println(why);
  cmd_pending = false;
}

/* Call each loop(); returns true if still pending */
bool pumpCommandUntilAck() {
  if (!cmd_pending) return false;

  if ((uint32_t)(millis() - last_tx_ms) >= RESEND_MS) {
    last_tx_ms = millis();
    radio.stopListening();
    bool ok = radio.write(&cmdTX, 1);
    radio.startListening();
    if (ok) Serial.println(F("link-ack"));
  }

  while (radio.isAckPayloadAvailable()) {
    uint8_t n = radio.getDynamicPayloadSize();
    if (n == 0 || n > 32) n = 32;
    uint8_t buf[32]; radio.read(buf, n);
    if (n >= sizeof(CmdAck)) {
      CmdAck ack; memcpy(&ack, buf, sizeof(ack));
      if (ack.tag == CMD_ACK_TAG && ack.echo == cmdTX) {
        Serial.print(F("ack-payload phase=")); Serial.println(ack.phase);
        finishCommand(F("CONFIRMED ✅"));
        return false;
      }
    }
  }

  if (radio.available()) {           // keep printing telemetry while pending
    TelemetryV1 t;
    if (rf_recvTelemetry(radio, t)) printTelemetryCSV(t);
    else {
      uint8_t junk[32]; uint8_t n = radio.getDynamicPayloadSize();
      if (n == 0 || n > sizeof(junk)) n = sizeof(junk);
      radio.read(junk, n);
    }
  }
  return true;
}
/* ----------------------------------------------------------------- */

void setup() {
  Serial.begin(115200);
  while (!Serial) {;}
  pinMode(LED_BUILTIN, OUTPUT);

  if (!radio.begin()) {
    Serial.println(F("RF24 init FAIL"));
    while (1) { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); delay(150); }
  }

  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(5, 15);
  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, RADIO_ADDRESS);
  radio.openWritingPipe(RADIO_ADDRESS);
  radio.startListening();

  printCsvHeader();
  Serial.println(F("GS ready. Type 'a' or 'l'."));
}

void loop() {
  if (Serial.available() && !cmd_pending) {
    char c = tolower(Serial.read());
    if (c == 'a' || c == 'l') beginCommand(c);
  }

  if (cmd_pending) {
    pumpCommandUntilAck();            // keep retrying until ACK, still logs telem
    return;                           // skip normal telem block while sending
  }

  if (radio.available()) {
    TelemetryV1 t;
    if (rf_recvTelemetry(radio, t)) {
      printTelemetryCSV(t);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    } else {
      uint8_t junk[32]; uint8_t n = radio.getDynamicPayloadSize();
      if (n == 0 || n > sizeof(junk)) n = sizeof(junk);
      radio.read(junk, n);
    }
  }
}
