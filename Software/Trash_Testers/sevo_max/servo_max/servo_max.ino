#include <Servo.h>

Servo servoA;
Servo servoB;

const int RED_LED   = 21;
const int GREEN_LED = 20;

const uint16_t SERVO_MIN_US = 1200;
const uint16_t SERVO_MAX_US = 3000;

const uint8_t  STEP_DEG = 1; 
const uint16_t HOLD_MS  = 10; 

void setup() {
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Serial.begin(115200);

  servoA.attach(5, SERVO_MIN_US, SERVO_MAX_US);   
  servoB.attach(6, SERVO_MIN_US, SERVO_MAX_US);

  delay(500);   

  for (uint8_t angle = 0; angle <= 180; angle += STEP_DEG) {
    digitalWrite(RED_LED, HIGH);       
    digitalWrite(GREEN_LED, LOW);

    servoA.write(angle);
    servoB.write(angle);
    Serial.println(angle);             
    delay(10);                         

    digitalWrite(RED_LED, LOW);       
    digitalWrite(GREEN_LED, HIGH);
    delay(HOLD_MS);
  }

   servoA.write(90);
  servoB.write(90);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);         
}

void loop() {}   
