#include <Servo.h>

Servo servoSmall;
Servo servoBig;
Servo ESCone;
Servo ESCtwo;

const int buzzerPin = 32;

void setup() {
    Serial.begin(9600);
    servoSmall.attach(5);
    servoBig.attach(6);
    ESCone.attach(7);
   ESCtwo.attach(8);
    armServos();
    armESC();

    pinMode(buzzerPin, OUTPUT);
    beepBuzzer();
}

void loop() {
    moveBothServos();
    delay(2000);
    moveBigServo();
    delay(2000);
    moveSmallServo();
    delay(2000);
}

void armESC() {
   ESCone.writeMicroseconds(1000); // Minimum throttle
   delay(2000); 
   ESCtwo.writeMicroseconds(1000); 
   delay(2000);
}

// Initialize servos at the middle position
void armServos() {
    servoSmall.write(90); 
    servoBig.write(90); 
    delay(2000);
}

// Function to beep the buzzer before movement
void beepBuzzer() {
    tone(buzzerPin, 1000, 200); // 1000Hz tone for 200ms
    delay(300);
}

// Function to move servos smoothly
void moveServoSmooth(Servo &servo, int start, int end, int stepDelay) {
    int step = (start < end) ? 1 : -1;
    for (int pos = start; pos != end; pos += step) {
        servo.write(pos);
        delay(stepDelay);
    }
    servo.write(end);
}

// Move both servos from 45 to 135 and back to 90
void moveBothServos() {
   // beepBuzzer();
    moveServoSmooth(servoSmall, 90, 45, 5);
    moveServoSmooth(servoBig, 90, 45, 5);
    delay(500);
    
    moveServoSmooth(servoSmall, 45, 135, 5);
    moveServoSmooth(servoBig, 45, 135, 5);
    delay(500);

    moveServoSmooth(servoSmall, 135, 90, 5);
    moveServoSmooth(servoBig, 135, 90, 5);
}

// Move only the big servo from 45 to 135 and back
void moveBigServo() {
    //beepBuzzer();
    moveServoSmooth(servoBig, 90, 45, 5);
    delay(500);
    moveServoSmooth(servoBig, 45, 135, 5);
    delay(500);
    moveServoSmooth(servoBig, 135, 90, 5);
}

// Move only the small servo from 45 to 135 and back
void moveSmallServo() {
    //beepBuzzer();
    moveServoSmooth(servoSmall, 90, 45, 5);
    delay(500);
    moveServoSmooth(servoSmall, 45, 135, 5);
    delay(500);
    moveServoSmooth(servoSmall, 135, 90, 5);
}
