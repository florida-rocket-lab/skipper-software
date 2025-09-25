//#include "FlightComputer.h"

//FlightComputer fc;

void setup() {
  Serial.begin(115200);
    Serial.println("hello pls init");

  //while(!Serial && millis()<3000){}
  //if (!fc.begin()) Serial.println("Init error!");
}

void loop() {
  //fc.update();   
  Serial.println("not cooked");    
}
