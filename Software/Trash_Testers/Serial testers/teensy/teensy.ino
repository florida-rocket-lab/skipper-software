void setup() {
  Serial.begin(9600);    
  Serial3.begin(9600);   
  randomSeed(micros());
  Serial.println("TEENSY: up and running on Serial3 TX=20");
}

void loop() {
  int d = random(0,10);
  Serial3.println(d);
  Serial.print("TX→Nano: "); Serial.println(d);
  delay(50);
}
