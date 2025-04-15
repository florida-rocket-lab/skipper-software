#ifndef SKIPPER_GNC_COMMUNICATION_H
#define SKIPPER_GNC_COMMUNICATION_H

#include "datatypes.h"
#include "arduino_compat.h"
#include "constants.h"
#include "cobs.h"
#include <type_traits>
#include <cstring>
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

enum class Status : unsigned char {
    NOMINAL,
    RADIO_INIT_FAILURE,
    RADIO_AVAILABILITY_FAILURE,
    RADIO_WRONG_HANDSHAKE_FAILURE,
    SERIAL_INIT_FAILURE,
    SERIAL_AVAILABILITY_FAILURE,
    SERIAL_WRONG_HANDSHAKE_FAILURE
};

class BaseCommunication {
public:
    BaseCommunication() = default;

    virtual bool ping() = 0;
    [[nodiscard]] virtual bool alive() const = 0;

    virtual void send(const BaseSerializable* data) = 0;
    virtual void send(UniquePtr<BaseSerializable> data) = 0;
    virtual UniquePtr<BaseSerializable> receive(size_t buffer_size) = 0;

    Status get_status() { return this->status; }

protected:
    Status status = Status::NOMINAL;
};

class RadioCommunication : virtual public BaseCommunication {
public:
    RadioCommunication(unsigned char ce_pin, unsigned char csn_pin, const uint8_t address[6], unsigned char rf_channel = 108);

    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const BaseSerializable* data) override;
    void send(UniquePtr<BaseSerializable> data) override;

    template <typename T>
    UniquePtr<T> receive();

private:
    mutable RF24 nrf24;
};

class UARTCommunication : virtual public BaseCommunication {
public:
    explicit UARTCommunication(HardwareSerial* serial_);
    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const BaseSerializable* data) override;
    void send(UniquePtr<BaseSerializable> data) override;

    UniquePtr<BaseSerializable> receive(size_t) override;

    template <typename T>
    UniquePtr<T> receive();

private:
    HardwareSerial* serial;
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


#endif // SKIPPER_GNC_COMMUNICATION_H
