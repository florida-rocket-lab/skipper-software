#ifndef SKIPPER_GNC_COMMUNICATION_H
#define SKIPPER_GNC_COMMUNICATION_H

#include "datatypes.h"
#include "arduino_compat.h"
#include "constants.h"
#include "cobs.h"
#include "packet.h"

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>  

enum class Status : uint8_t {
    NOMINAL,
    RADIO_INIT_FAILURE,
    RADIO_AVAILABILITY_FAILURE,
    RADIO_WRONG_HANDSHAKE_FAILURE,
    SERIAL_INIT_FAILURE,
    SERIAL_AVAILABILITY_FAILURE,
    SERIAL_WRONG_HANDSHAKE_FAILURE,
    OK
};

class BaseCommunication {
public:
    virtual ~BaseCommunication() = default;
    virtual bool ping() = 0;
    virtual bool alive() = 0;
    virtual void send(const BaseSerializable* data, uint8_t rx, uint8_t tx, uint8_t cmd) = 0;
    virtual void send(UniquePtr<BaseSerializable> data, uint8_t rx, uint8_t tx, uint8_t cmd) = 0;
    virtual UniquePtr<BaseSerializable> receive(size_t buffer_size) = 0;
    Status get_status() const { return status; }
protected:
    Status status = Status::NOMINAL;
};

class RadioCommunication : public BaseCommunication {
public:
    RadioCommunication(uint8_t ce_pin, uint8_t csn_pin,
                       const uint8_t address_[6],
                       uint8_t rf_channel = RADIO_COMMUNICATION_CHANNEL);
    void start_rx() { nrf.startListening(); }
    void start_tx() { nrf.stopListening(); }
    void init();
    bool ping() override;
    bool alive() override; 
    void send(const BaseSerializable* data, uint8_t rx, uint8_t tx, uint8_t cmd) override;
    void send(UniquePtr<BaseSerializable> data, uint8_t rx, uint8_t tx, uint8_t cmd) override;
    UniquePtr<BaseSerializable> receive(size_t buffer_size) override;
    template<typename T> UniquePtr<T> receive();
private:
    RF24       nrf;
    const uint8_t* address;
    uint8_t    rf_channel;
};

class UARTCommunication : public BaseCommunication {
public:
    explicit UARTCommunication(Stream* serial_);
    Stream* getSerial() const { return serial; }
    bool ping() override;
    bool alive() override; 
    void send(const BaseSerializable* data, uint8_t rx, uint8_t tx, uint8_t cmd) override;
    void send(UniquePtr<BaseSerializable> data, uint8_t rx, uint8_t tx, uint8_t cmd) override;
    UniquePtr<BaseSerializable> receive(size_t buffer_size) override;
    template<typename T> UniquePtr<T> receive();
private:
    Stream* serial;
};

class USBCommunication : public UARTCommunication {
public:
    USBCommunication();
};

class RTX1Communication : public UARTCommunication {
public:
    RTX1Communication();
};

class RTX3Communication : public UARTCommunication {
public:
    RTX3Communication();
};

#endif 

