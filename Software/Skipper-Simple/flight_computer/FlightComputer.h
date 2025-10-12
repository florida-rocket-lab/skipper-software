#pragma once

#include <Arduino.h>
#include <MPU6050.h>
#include <MadgwickAHRS.h>
#include <RF24.h>
#include <Servo.h>
#include <SD.h>
#include <Adafruit_BMP280.h>

#include "skipper_lqi_for_export.h"
#include "packet.h"
#include "constants.h"
#include "Converter.h"
#include "communication.h"

enum class FlightPhase : uint8_t { IDLE, COUNTDOWN, ASCENT, HOVER, DESCENT, DISARMED };

class FlightComputer {
public:
    bool begin();
    void update();
    FlightPhase phase() const { return fcPhase; }

private:
    MPU6050        mpu;
    Adafruit_BMP280 bmp;
    Madgwick       ahrs;

    skipperLQR     ctrl;
    skipperLQR::ExtU_skipper_lqi_for_export_T ctrlU{};

    RF24           radio{TEENSY_CE_PIN, TEENSY_CSN_PIN};
    Servo          esc1, esc2, servoUp, servoLo;
    File           logFile;

    float          alt0 = 0.0f;
    float          altFilt = 0.0f;

    static constexpr uint32_t RUN_WINDOW_MS = 60000;
    bool           runActive = false;
    uint32_t       runStopMs = 0;

    bool initIMU();
    bool initBaro();
    bool initRadio();
    bool initSD();
    void initActuators();
    void setLED(bool g, bool r);

    struct IMURaw { int16_t ax, ay, az, gx, gy, gz; };
    IMURaw readIMU();
    void   updateAHRS(const IMURaw& r);
    void   buildPlantState(const IMURaw& r, float alt_m, float dt);
    void   setReferenceFeet(float alt_ft, float cross_ft, float down_ft);

    void pushState();
    void runController();
    void disarmMotors();

    void sendTelemetry(const IMURaw& imu, float alt_m);
    void logFrame(const IMURaw& r, float alt);

    void handleRadioCmd();
    void handleSerialCmd();

    // State vars
    FlightPhase fcPhase = FlightPhase::IDLE;
    uint32_t    phaseStartMs = 0;   
    uint32_t    lastCtrlUs   = 0;   
    uint32_t    lastFlushMs  = 0;

    float       altRef = 0.0f;    
    uint16_t    pwmEsc = 1000;
    uint16_t    pwmUp  = 1500;
    uint16_t    pwmLo  = 1500;

    float       velX = 0.0f, velY = 0.0f;
    float       rollDegCmd = 0.0f, pitchDegCmd = 0.0f;
};
