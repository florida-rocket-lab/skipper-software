#include <Servo.h>

Servo ESC1;
Servo ESC2;

const int RED_LED_PIN   = 21;  
const int GREEN_LED_PIN = 20;  

 const int minPercent   = 50;  
const int maxPercent   = 75;  
const int stepPercent  = 5;   
const int settleTime_ms = 1000;  
 
const int ESC_MIN_US = 1000;    
1500
12500
250
const int ESC_MAX_US = 2000;    

void setup() {
  pinMode(RED_LED_PIN,   OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  Serial.begin(115200);

  ESC1.attach(7);   
  ESC2.attach(8);

  armESCs();
  runSweep();
}

void loop() {}   

 void armESCs() {
  ESC1.writeMicroseconds(ESC_MIN_US);
  ESC2.writeMicroseconds(ESC_MIN_US);
  delay(4000);  
}

void runSweep() {
  for (int pct = minPercent; pct <= maxPercent; pct += stepPercent) {
    
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);

    int pulse = map(pct, 0, 100, ESC_MIN_US, ESC_MAX_US);
    ESC1.writeMicroseconds(pulse);
    ESC2.writeMicroseconds(pulse);

    delay(250); 
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);  

    Serial.print("Throttle = ");
    Serial.print(pct);
    Serial.print("%  (");
    Serial.print(pulse);
    Serial.println(" µs)");
    delay(settleTime_ms);     

    digitalWrite(GREEN_LED_PIN, LOW);
  }

   ESC1.writeMicroseconds(ESC_MIN_US);
  ESC2.writeMicroseconds(ESC_MIN_US);
  digitalWrite(RED_LED_PIN, HIGH);  
}
