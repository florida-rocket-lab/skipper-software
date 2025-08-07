#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>
#include <Skipper.h>
#include <printf.h>

SoftwareSerial teenLink(NANO_RX1_PIN, NANO_TX1_PIN);
RF24 radio(NANO_CE_PIN, NANO_CSN_PIN);

// Receives IMU frame over UART
bool uart_recvIMUFrame(Stream& s, IMUFrame& out) {
    while (s.available() && s.peek() != IMU_START) {
        s.read(); // Skip silently
    }
    if (!s.available()) return false;

    uint8_t raw[32];
    if (s.readBytes(raw, IMU_PACKET_SIZE) != IMU_PACKET_SIZE) {
        return false;
    }

    if (raw[0] != IMU_START || raw[1] != IMU_FRAME_LEN) {
        return false;
    }
    if (compute_crc8(raw + 2, IMU_FRAME_LEN - 1) != raw[IMU_FRAME_LEN + 1]) {
        return false;
    }

    memcpy(&out, raw + 2, sizeof(IMUFrame));
    return true;
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    teenLink.begin(57600); // Lowered from 9600
    radio.begin();
    radio.setRetries(15, 15);
    radio.setAutoAck(false);
    radio.setChannel(RADIO_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(RADIO_ADDRESS);
    radio.openReadingPipe(0, RADIO_ADDRESS);
    radio.disableDynamicPayloads();
    radio.setPayloadSize(IMU_PACKET_SIZE);
    radio.startListening();
    radio.flush_tx();
    radio.flush_rx();
    printf_begin();
    delay(1000);
    Serial.println(F("Nano relay ready"));
}

void loop() {
    IMUFrame imu;
    if (uart_recvIMUFrame(teenLink, imu)) {
        Serial.print(F("A[g] "));
        Serial.print(imu.ax, 3);
        Serial.print(' ');
        Serial.print(imu.ay, 3);
        Serial.print(' ');
        Serial.print(imu.az, 3);
        Serial.print(F("   G[rad/s] "));
        Serial.print(imu.gx, 3);
        Serial.print(' ');
        Serial.print(imu.gy, 3);
        Serial.print(' ');
        Serial.println(imu.gz, 3);

        radio.stopListening();
        radio.flush_tx();
        uint8_t debugBuf[IMU_PACKET_SIZE];
        debugBuf[0] = IMU_START;
        debugBuf[1] = IMU_FRAME_LEN;
        memcpy(debugBuf + 2, &imu, sizeof(IMUFrame));
        debugBuf[IMU_PACKET_SIZE - 1] = compute_crc8(debugBuf + 2, sizeof(IMUFrame));
        Serial.print(F("Nano TX: "));
        for (int i = 0; i < IMU_PACKET_SIZE; i++) {
            Serial.print(debugBuf[i], HEX);
            Serial.print(' ');
        }
        Serial.println();
        bool ok = rf_sendIMU(radio, imu);
        radio.startListening();
        Serial.println(ok ? F("TX ok") : F("TX fail"));
    }
}