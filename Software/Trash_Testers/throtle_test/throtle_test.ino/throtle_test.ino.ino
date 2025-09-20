// #include <Servo.h>

// Servo ESC1, ESC2;

// const int ESC1_PIN = 7;
// const int ESC2_PIN = 8;

// const int RED_LED_PIN   = 32;
// const int GREEN_LED_PIN = 31;

// const int ESC_MIN_US = 1000;
// const int ESC_MAX_US = 2000;

// int startPercent   = 10;
// int maxPercent     = 50;
// int stepPercent    = 5;
// int settleTime_ms  = 500;
// int rampStep_us    = 10;    // microsecond increment per ramp step
// int rampDelay_ms   = 4;     // delay between ramp steps

// void writeBothUS(int us) {
//   us = constrain(us, ESC_MIN_US, ESC_MAX_US);
//   ESC1.writeMicroseconds(us);
//   ESC2.writeMicroseconds(us);
// }

// void rampTo(int target_us) {
//   target_us = constrain(target_us, ESC_MIN_US, ESC_MAX_US);
//   int current = ESC_MIN_US;
//   // read back last written value would be nicer, but Servo has no getter
//   for (current = ESC_MIN_US; current != target_us; ) {
//     current += (current < target_us) ? rampStep_us : -rampStep_us;
//     if ((current < ESC_MIN_US) || (current > ESC_MAX_US)) break;
//     writeBothUS(current);
//     delay(rampDelay_ms);
//     if (Serial.available()) {  // quick kill
//       Serial.read();
//       writeBothUS(ESC_MIN_US);
//       return;
//     }
//   }
// }

// void armESCs() {
//   writeBothUS(ESC_MIN_US);
//   delay(4000);
// }

// void runSweep() {
//   for (int pct = startPercent; pct <= maxPercent; pct += stepPercent) {
//     digitalWrite(RED_LED_PIN, HIGH);
//     digitalWrite(GREEN_LED_PIN, LOW);

//     int target = map(pct, 0, 100, ESC_MIN_US, ESC_MAX_US);
//     target = constrain(target, ESC_MIN_US, ESC_MAX_US);

//     rampTo(target);

//     digitalWrite(RED_LED_PIN, LOW);
//     digitalWrite(GREEN_LED_PIN, HIGH);

//     Serial.print("Throttle ");
//     Serial.print(pct);
//     Serial.print("%  => ");
//     Serial.print(target);
//     Serial.println(" us");

//     unsigned long t0 = millis();
//     while (millis() - t0 < (unsigned long)settleTime_ms) {
//       if (Serial.available()) {  // quick kill during settle
//         Serial.read();
//         writeBothUS(ESC_MIN_US);
//         digitalWrite(GREEN_LED_PIN, LOW);
//         digitalWrite(RED_LED_PIN, HIGH);
//         return;
//       }
//       delay(5);
//     }

//     digitalWrite(GREEN_LED_PIN, LOW);
//   }

//   writeBothUS(ESC_MIN_US);
//   digitalWrite(RED_LED_PIN, HIGH);
// }

// void setup() {
//   pinMode(RED_LED_PIN, OUTPUT);
//   pinMode(GREEN_LED_PIN, OUTPUT);
//   digitalWrite(RED_LED_PIN, LOW);
//   digitalWrite(GREEN_LED_PIN, LOW);

//   Serial.begin(115200);
//   // Attach with explicit limits for robustness
//   ESC1.attach(ESC1_PIN, ESC_MIN_US, ESC_MAX_US);
//   ESC2.attach(ESC2_PIN, ESC_MIN_US, ESC_MAX_US);

//   armESCs();
//   runSweep();
// }

// void loop() { /* one-shot test in setup */ }


#include <Servo.h>

Servo ESC1, ESC2;

const int ESC1_PIN = 7;
const int ESC2_PIN = 8;

const int ESC_MIN_US = 1000;   // ESC idle
const int ESC_MAX_US = 2000;   // ESC full throttle

const int RAMP_STEP_US  = 10;  // microsecond step size
const int RAMP_DELAY_MS = 20;  // delay between steps
const int HOLD_TIME_MS  = 10000; // hold at 100% for 2 sec

void writeBoth(int us) {
  us = constrain(us, ESC_MIN_US, ESC_MAX_US);
  ESC1.writeMicroseconds(us);
  ESC2.writeMicroseconds(us);
}

void setup() {
  Serial.begin(115200);

  ESC1.attach(ESC1_PIN, ESC_MIN_US, ESC_MAX_US);
  ESC2.attach(ESC2_PIN, ESC_MIN_US, ESC_MAX_US);

  // Arm ESCs at minimum
  writeBoth(ESC_MIN_US);
  delay(4000);  

  Serial.println("Starting ramp...");

  // Ramp smoothly up to max
  for (int us = ESC_MIN_US; us <= ESC_MAX_US; us += RAMP_STEP_US) {
    writeBoth(us);
    delay(RAMP_DELAY_MS);
  }

  Serial.println("Holding full throttle...");
  delay(HOLD_TIME_MS);

  // Back to idle
  Serial.println("Back to idle.");
  writeBoth(ESC_MIN_US);
}

void loop() {
  // nothing — runs once
}


