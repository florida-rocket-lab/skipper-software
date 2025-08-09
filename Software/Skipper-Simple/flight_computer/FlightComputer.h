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

/* 
  High-level timeline                                           
  COUNTDOWN :  0-5 s buzzer / red+green solid                                
  ASCENT    :  5-10 s climb to 1 m                                           
  HOVER     : 10-12 s hold 1 m                                               
  DESCENT   : 12-17 s ramp to ground                                         
  DISARMED  : ≥17 s motors cut, green LED only                                
 */

enum class FlightPhase : uint8_t { IDLE, COUNTDOWN, ASCENT, HOVER, DESCENT, DISARMED };

class FlightComputer {
public:
    bool begin();
    void update();
    FlightPhase phase() const { return fcPhase; }

private:
    //  Peripherals  
    MPU6050   mpu;
    Adafruit_BMP280 bmp;
    Madgwick  ahrs;
    skipper_lqi_for_export ctrl;
    RF24      radio{TEENSY_CE_PIN, TEENSY_CSN_PIN};
    Servo     esc1, esc2, servoUp, servoLo;
    File      logFile;
    float alt0 = 0.0f, altFilt = 0.0f;


    static constexpr uint32_t RUN_WINDOW_MS = 15000; // 15 s from ARM
    bool     runActive = false;
    uint32_t runStopMs = 0;


    // Init helpers 
    bool initIMU();
    bool initBaro();
    bool initRadio();
    bool initSD();
    void initActuators();
    void setLED(bool g, bool r);

    //Sensors 
    struct IMURaw { int16_t ax, ay, az, gx, gy, gz; };
    IMURaw  readIMU();
    void    updateAHRS(const IMURaw&);
    void buildPlantState(const IMURaw& r, float alt, float dt);
    void setReferenceFeet(float alt_ft, float cross_ft, float down_ft);


    // Control and output
    void pushState();
    void runController();
    void disarmMotors();

    // Telemetry 
    void sendTelemetry(const IMURaw&, float alt);
    void logFrame(const IMURaw&, float alt);

    //Commands
    void handleRadioCmd();
    void handleSerialCmd();

    //  State Vars= 
    FlightPhase fcPhase = FlightPhase::IDLE;
    uint32_t    phaseStartMs = 0;       // millis at entry of current phase
    uint32_t    lastCtrlUs   = 0;       // micro-tick for 200 Hz loop
    uint32_t    lastFlushMs  = 0;
    float       altRef       = 0.0f;    // metres – fed to controller
    uint16_t    pwmEsc  = 1000, pwmUp = 1500, pwmLo = 1500;
    float       velX = 0, velY = 0;
    float       rollDegCmd = 0.0f, pitchDegCmd = 0.0f;
};
