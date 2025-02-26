#include "RadioConnection.h"

RadioConnection::RadioConnection(RF24& radioInstance) : nrf24(radioInstance) {}

void RadioConnection::init(const byte address[6]) {
    if (!nrf24.begin()) {
        Serial.println("NRF24 Initialization Failed!");
    }
    nrf24.setChannel(108);
    nrf24.setDataRate(RF24_250KBPS); // possible values: RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
    nrf24.setPALevel(RF24_PA_LOW); // possible values: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX 
    nrf24.openWritingPipe(address); // writing pipe address 
    nrf24.openReadingPipe(1, address); // reading pipe address
    nrf24.stopListening(); // default as transmitter
}

// is the data avaiable? like to go on a date?
bool RadioConnection::available() {
    return nrf24.available();
}

//  send `Ground2Teensy` (command to vehicle)
void RadioConnection::send(const Ground2Teensy& command) {
    uint8_t buffer[4];
    command.serialize(buffer);
    nrf24.write(buffer, sizeof(buffer));
    nrf24.available();
}

// send `Teensy2Ground` (telemetry from vehicle)
void RadioConnection::send(const Teensy2Ground& flightData) {
    uint8_t buffer[sizeof(Teensy2Ground)];
    flightData.serialize(buffer);
    nrf24.write(buffer, sizeof(buffer));
    nrf24.available();
}

// receive `Ground2Teensy` (Command)
bool RadioConnection::receive(Ground2Teensy& command) {
    if (!nrf24.available()) {
        return false;
    }

    uint8_t buffer[sizeof(Ground2Teensy)];
    nrf24.read(buffer, sizeof(buffer));  
    return Ground2Teensy::deserialize(buffer, command.command); 
}



// test NRF24 Connection
bool RadioConnection::testConnection() {
    uint8_t testMessage[] = "PING";
    nrf24.write(testMessage, sizeof(testMessage)); 

    uint8_t buf[4];
    uint8_t len = sizeof(buf);

    unsigned long start = millis();  
    while (!nrf24.available()) {    
        if (millis() - start > 500) {  
            return false;  
        }
    }

    nrf24.read(buf, sizeof(buf)); 

    return (strncmp((char*)buf, "PONG", 4) == 0);
}
