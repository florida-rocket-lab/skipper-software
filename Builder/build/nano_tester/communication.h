// communication.h
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
#include <SoftwareSerial.h>   // for the AVR fallback

enum class Status : unsigned char {
    NOMINAL,
    RADIO_INIT_FAILURE,
    RADIO_AVAILABILITY_FAILURE,
    RADIO_WRONG_HANDSHAKE_FAILURE,
    SERIAL_INIT_FAILURE,
    SERIAL_AVAILABILITY_FAILURE,
    SERIAL_WRONG_HANDSHAKE_FAILURE,
    OK  
};

struct PacketHeader {
    uint8_t start_byte = 0xAA;
    uint16_t length;
    uint8_t receiver_id;
    uint8_t sender_id;
    uint8_t command_id;
};

class BaseCommunication {
public:
    BaseCommunication() = default;
    virtual bool ping() = 0;
    [[nodiscard]] virtual bool alive() const = 0;
    virtual void send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) = 0;
    virtual void send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) = 0;
    virtual UniquePtr<BaseSerializable> receive(size_t buffer_size) = 0;
    Status get_status() { return this->status; }
protected:
    Status status = Status::NOMINAL;
};

class RadioCommunication : virtual public BaseCommunication {
public:
    RadioCommunication(uint8_t ce_pin, uint8_t csn_pin, const uint8_t address[6], uint8_t rf_channel = RADIO_COMMUNICATION_CHANNEL);
    void start_rx() { nrf24.startListening(); }
    void start_tx() { nrf24.stopListening();  }
    void init();
    bool ping() override;
    [[nodiscard]] bool alive() const override;
    void send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) override;
    void send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) override;
    UniquePtr<BaseSerializable> receive(size_t buffer_size) override;
    template <typename T>
    UniquePtr<T> receive();
private:
    mutable RF24 nrf24;
    const uint8_t* address;
    uint8_t rf_channel;
};

class UARTCommunication : virtual public BaseCommunication {
public:
    explicit UARTCommunication(HardwareSerial* serial_);
    explicit UARTCommunication(SoftwareSerial* serial_);
    Stream* getSerial() { return serial; }
    bool ping() override;
    [[nodiscard]] bool alive() const override;
    void send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) override;
    void send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) override;
    UniquePtr<BaseSerializable> receive(size_t) override;
    template <typename T>
    UniquePtr<T> receive();
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

#endif // SKIPPER_GNC_COMMUNICATION_H
