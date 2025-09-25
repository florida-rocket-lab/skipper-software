#include <Servo.h>

/* CONFIG */
const int A_MIN_US = 1100;
const int A_MAX_US = 2400;
const int B_MIN_US = 1100;
const int B_MAX_US = 2400;

const int STEP_US  = 10;
const int HOLD_MS  = 15;

const int RED_LED = 32, GREEN_LED = 31;
const int SERVO_A_PIN = 5, SERVO_B_PIN = 6;

Servo servoA, servoB;
inline void led(bool g, bool r){ digitalWrite(GREEN_LED,g); digitalWrite(RED_LED,r); }

static inline int lerpB(int pw){
  // linear map A range -> B range using 32-bit math
  const int aSpan = A_MAX_US - A_MIN_US;
  const int bSpan = B_MAX_US - B_MIN_US;
  return B_MIN_US + (int32_t)(pw - A_MIN_US) * bSpan / aSpan;
}

void setup(){
  pinMode(RED_LED,OUTPUT); pinMode(GREEN_LED,OUTPUT);
  Serial.begin(115200);
  servoA.attach(SERVO_A_PIN, A_MIN_US, A_MAX_US);
  servoB.attach(SERVO_B_PIN, B_MIN_US, B_MAX_US);
  led(1,0); delay(300); led(0,0);
}

void loop() {
  const int centerA = (A_MIN_US + A_MAX_US) / 2;
  const int centerB = (B_MIN_US + B_MAX_US) / 2;

  // --- Servo A ---
  servoB.writeMicroseconds(centerB);            // park B at center
  servoA.writeMicroseconds(centerA); delay(500);

  for (int pw = centerA; pw <= A_MAX_US; pw += STEP_US) { // center -> right
    servoA.writeMicroseconds(pw); led(0,1); delay(HOLD_MS);
  }
  servoA.writeMicroseconds(centerA); led(1,1); delay(500); // back to center

  for (int pw = centerA; pw >= A_MIN_US; pw -= STEP_US) { // center -> left
    servoA.writeMicroseconds(pw); led(0,1); delay(HOLD_MS);
  }
  servoA.writeMicroseconds(centerA); led(1,1); delay(800); // settle

  // --- Servo B ---
  servoA.writeMicroseconds(centerA);            // park A at center
  servoB.writeMicroseconds(centerB); delay(500);

  for (int pw = centerB; pw <= B_MAX_US; pw += STEP_US) { // center -> right
    servoB.writeMicroseconds(pw); led(0,1); delay(HOLD_MS);
  }
  servoB.writeMicroseconds(centerB); led(1,1); delay(500);

  for (int pw = centerB; pw >= B_MIN_US; pw -= STEP_US) { // center -> left
    servoB.writeMicroseconds(pw); led(0,1); delay(HOLD_MS);
  }
  servoB.writeMicroseconds(centerB); led(1,1); delay(1200); // pause before repeat
}





// void loop() {
//   servoA.writeMicroseconds(1500);
//   servoB.writeMicroseconds(1500);

//   led(1, 0); // green LED on
//   delay(1000); // just hold position
// }

// #include <Servo.h>
// Servo s;
// void setup(){ s.attach(5); delay(300); }
// void loop(){
//   s.writeMicroseconds(800); delay(1500);
//   s.writeMicroseconds(1500); delay(1500);
//   s.writeMicroseconds(2200); delay(1500);
// }

