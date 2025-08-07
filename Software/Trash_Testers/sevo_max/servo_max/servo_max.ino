/*──────────────────────── Servo Range Calibrator ────────────────────────*
 * 1. Edit MIN_US / MAX_US for each servo in the CONFIG block below.      *
 * 2. Upload → the sketch sweeps both servos from min → max → min.        *
 * 3. Watch the motion (or the Serial plot) and note the true limits.     *
 * 4. Tweak the numbers, re-upload, repeat until happy.                   *
 *------------------------------------------------------------------------*/

#include <Servo.h>

/* ───────────── CONFIG – edit here ───────────── */
static const uint16_t A_MIN_US = 1100;   // lower bound for Servo A
static const uint16_t A_MAX_US = 2400;   // upper bound for Servo A
static const uint16_t B_MIN_US = 1200;   // lower bound for Servo B
static const uint16_t B_MAX_US = 2500;   // upper bound for Servo B

static const uint16_t STEP_US  = 10;     // pulse-width step each update
static const uint16_t HOLD_MS  = 15;     // delay between steps
/* ────────────────────────────────────────────── */

static const int RED_LED   = 21;
static const int GREEN_LED = 20;
static const int SERVO_A_PIN = 5;
static const int SERVO_B_PIN = 6;

Servo servoA, servoB;

/* helpers */
void led(bool g, bool r) { digitalWrite(GREEN_LED, g); digitalWrite(RED_LED, r); }

void setup() {
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Serial.begin(115200);

  servoA.attach(SERVO_A_PIN, A_MIN_US, A_MAX_US);
  servoB.attach(SERVO_B_PIN, B_MIN_US, B_MAX_US);

  led(1,0); delay(300);                // green = ready
  led(0,0);
}

void loop() {
  /* sweep up */
  for (uint16_t pw = A_MIN_US; pw <= A_MAX_US; pw += STEP_US) {
    servoA.writeMicroseconds(pw);
    servoB.writeMicroseconds(map(pw, A_MIN_US, A_MAX_US, B_MIN_US, B_MAX_US));
    Serial.println(pw);                // view as graph if you like
    led(0,1); delay(HOLD_MS);          // red LED while moving
  }

  /* hold at max */
  led(1,1); delay(500);

  /* sweep down */
  for (uint16_t pw = A_MAX_US; pw >= A_MIN_US; pw -= STEP_US) {
    servoA.writeMicroseconds(pw);
    servoB.writeMicroseconds(map(pw, A_MIN_US, A_MAX_US, B_MIN_US, B_MAX_US));
    Serial.println(pw);
    led(0,1); delay(HOLD_MS);
  }

  /* hold at min */
  led(1,1); delay(500);
}
