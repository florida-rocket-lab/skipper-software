
// Wiring: 
//CE to D9
//CSN to D10  
//SCK to D13
//MOSI to D11 
//MISO to D12

// Green LED = D5 
// Red LED   =  D6
#include <SPI.h>
#include <RF24.h>
#include "constants.h"
#include "communication.h"

RF24 radio(UNO_CE_PIN, UNO_CSN_PIN);
const uint64_t pipe = *(uint64_t*)RADIO_ADDRESS;

const uint8_t LED_PIN = UNO_SUCCESS_PIN;         // D5

enum class GSMode : uint8_t { IDLE, WAIT_ACK, TELEMETRY, RADIO_FAIL };
GSMode mode = GSMode::IDLE;

char      pendingCmd = '\0';
uint32_t  lastCmdMs  = 0;
constexpr uint16_t CMD_INTERVAL_MS = 200;

void fatalRadioFail() {
  mode = GSMode::RADIO_FAIL;
  digitalWrite(LED_PIN, HIGH);                  // solid ON
  Serial.println(F("‼  Radio lost  ‼"));
}
bool ensureRadioAlive() {
  if (radio.isChipConnected()) return true;
  fatalRadioFail();
  return false;
}
void sendCmd(char c) {
  radio.stopListening();
  radio.write(&c, 1);
  radio.startListening();
  Serial.print(F("TX cmd: ")); Serial.println(c);
}
void printFrame(const IMUFrame& f) {
  Serial.print(F("ax=")); Serial.print(f.ax, 4);
  Serial.print(F(" ay=")); Serial.print(f.ay, 4);
  Serial.print(F(" az=")); Serial.print(f.az, 4);
  Serial.print(F(" gx=")); Serial.print(f.gx, 4);
  Serial.print(F(" gy=")); Serial.print(f.gy, 4);
  Serial.print(F(" gz=")); Serial.println(f.gz, 4);
}

void initRadioBlock() {
  pinMode(LED_PIN, OUTPUT);
  Serial.println(F("Initialising nRF24L01…"));
  while (!radio.begin()) {                      // retry forever
    Serial.println(F("nRF24 not responding — check wiring/power"));
    digitalWrite(LED_PIN, HIGH); delay(200);
    digitalWrite(LED_PIN, LOW);  delay(800);
  }
  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  Serial.println(F("=== RF24 register dump ==="));
  radio.printDetails();
  Serial.println(F("=========================="));

  if (!ensureRadioAlive()) return;             
  Serial.println(F("Ground-station ready — type 'a' (arm) or 'l' (land)"));
}

void setup() {
  Serial.begin(115200);
  initRadioBlock();
}

void loop() {
  if (mode == GSMode::RADIO_FAIL) return;       // nothing works without RF

  // keyboard input 
  if (Serial.available()) {
    char c = tolower(Serial.read());
    if ((c == 'a' || c == 'l') && mode == GSMode::IDLE) {
      pendingCmd = c;
      mode       = GSMode::WAIT_ACK;
      lastCmdMs  = 0;                           // force immediate send
    }
  }

  // resend command until ACK
  if (mode == GSMode::WAIT_ACK && millis() - lastCmdMs >= CMD_INTERVAL_MS) {
    if (!ensureRadioAlive()) return;
    sendCmd(pendingCmd);
    lastCmdMs = millis();
  }

  //radio receive
  if (radio.available()) {
    if (!ensureRadioAlive()) return;

    /*  maybe it's an ACK */
    if (mode == GSMode::WAIT_ACK) {
      char ack; radio.read(&ack, 1);
      if (ack == 'K') {
        Serial.println(F("ACK received — telemetry streaming…"));
        mode = GSMode::TELEMETRY;
      }
      return;
    }

    /*   telemetry */
    if (mode == GSMode::TELEMETRY) {
      IMUFrame f;
      if (rf_recvIMU(radio, f)) {
        digitalWrite(LED_PIN, HIGH);
        printFrame(f);
        digitalWrite(LED_PIN, LOW);
      } else {
        radio.flush_rx();                      // bad CRC
      }
    }
  }
}
