#ifndef SKIPPER_GNC_COMMUNICATION_H
#define SKIPPER_GNC_COMMUNICATION_H

#include "arduino_compat.h"
#include <type_traits>
#include <cstring> 
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "constants.h"

enum class Status : unsigned char
{
    NOMINAL,
    RADIO_INIT_FAILURE,
    RADIO_AVAILABILITY_FAILURE,
    RADIO_WRONG_HANDSHAKE_FAILURE,
    SERIAL_INIT_FAILURE,
    SERIAL_AVAILABILITY_FAILURE,
    SERIAL_WRONG_HANDSHAKE_FAILURE
};

class BaseCommunication
{
public:
    BaseCommunication() = default;

    virtual bool ping() = 0;  // Handshake to see if the connection is live.
    [[nodiscard]] virtual bool alive() const = 0;  // Check to see if the connection is still available.

    virtual void send(const BaseSerializable* data) = 0;  // Every communication protocol needs to be able to send data.
    virtual void send(UniquePtr<BaseSerializable> data) = 0;
    virtual UniquePtr<BaseSerializable> receive(size_t buffer_size) = 0;  // Every communication protocol needs to receive and **deserialize** the data.

    Status get_status() {return this->status; };

protected:
    Status status = Status::NOMINAL;
};

class RadioCommunication: virtual public BaseCommunication
{
public:
    RadioCommunication(unsigned char ce_pin, unsigned char csn_pin, const uint8_t address[6] = (const uint8_t *)RADIO_COMMUNICATION_ADDRESS, unsigned char rf_channel = 108);

    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const BaseSerializable* data) override;
    void send(UniquePtr<BaseSerializable> data) override;

    template <typename T>
    UniquePtr<T> receive();

private:
    mutable RF24 nrf24{};
};

class UARTCommunication: virtual public BaseCommunication
{
public:
    explicit UARTCommunication(HardwareSerial* serial);

    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const BaseSerializable* data) override;
    void send(UniquePtr<BaseSerializable> data) override;

    template <typename T>
    UniquePtr<T> receive();

private:
    HardwareSerial* serial = nullptr;
};

class USBCommunication: virtual public UARTCommunication
{
    USBCommunication(): UARTCommunication(Serial) {};
};

class RTX1Communication: virtual public UARTCommunication
{
    RTX1Communication(): UARTCommunication(Serial1) {};
};

class RTX3Communication: virtual public UARTCommunication
{
    RTX3Communication(): UARTCommunication(Serial3) {};
};



// ======================== BEGIN COMMUNICATION CONSTRUCTORS ========================

RadioCommunication::RadioCommunication(const unsigned char ce_pin, const unsigned char csn_pin, const uint8_t address[6], const unsigned char rf_channel): BaseCommunication{}, nrf24(ce_pin, csn_pin)
{

    if (!nrf24.begin())
    {
        this->status = Status::RADIO_INIT_FAILURE;
        return;
    }

    nrf24.setChannel(rf_channel);  // Straight up ripped from Jose Reynaldo's original implementation.
    nrf24.setDataRate(RF24_250KBPS); // possible values: RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
    nrf24.setPALevel(RF24_PA_LOW); // possible values: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
    nrf24.openWritingPipe(reinterpret_cast<const uint8_t*>(address));    // writing pipe address
    nrf24.openReadingPipe(1, reinterpret_cast<const uint8_t*>(address)); // reading pipe address
    nrf24.stopListening(); // default as transmitter
}
UARTCommunication::UARTCommunication(HardwareSerial* serial_): serial(serial_)
{
    serial->begin(9600);
    if (!(*serial))
    {
        this->status = Status::SERIAL_INIT_FAILURE;
        return;
    }
}

// ======================== END COMMUNICATION CONSTRUCTORS ========================



// ======================== START COMMUNICATION SENDS ========================

void RadioCommunication::send(const BaseSerializable* data)
{
    Pair<UniquePtr<char[]>, unsigned int> data_buffer = data->serialize();
    if (!this->alive())
    {
        this->status = Status::RADIO_AVAILABILITY_FAILURE;
        return;
    }
    nrf24.write(data_buffer.first.get(), data_buffer.second);
    nrf24.available();
}
void UARTCommunication::send(const BaseSerializable* data)
{
    Pair<UniquePtr<char[]>, unsigned int> data_buffer = data->serialize();
    serial->write(data_buffer.first.get(), data_buffer.second);
}
void RadioCommunication::send(const UniquePtr<BaseSerializable> data)
{
    this->send(data.get());
}
void UARTCommunication::send(UniquePtr<BaseSerializable> data)
{
    this->send(data.get());
}

// ======================== END COMMUNICATION SENDS ========================



// ======================== START COMMUNICATION RECEIVES ========================

template <typename T>
UniquePtr<T> RadioCommunication::receive()
{
    UniquePtr<char[]> data_buffer = make_unique<char[]>(T::BUFFER_SIZE);
    unsigned long start = millis();
    while (!this->alive())
    {
        if (millis() - start > MESSAGE_TIMEOUT_MS)
        {
            this->status = Status::RADIO_AVAILABILITY_FAILURE;
            return {};
        }
    }

    nrf24.read(data_buffer.get(), T::BUFFER_SIZE);
    return {Move(data_buffer)};
}
template <typename T>
UniquePtr<T> UARTCommunication::receive()
{
    UniquePtr<char[]> data_buffer = make_unique<char[]>(T::BUFFER_SIZE);

    unsigned long start = millis();
    while (serial->available() < T::BUFFER_SIZE)  // Serial doesn't use null-terminator; total buffer should be 4-long.
    {
        if (millis() - start > MESSAGE_TIMEOUT_MS)
        {
            this->status = Status::SERIAL_AVAILABILITY_FAILURE;
            return {};
        }
    }

    serial->readBytes(data_buffer.get(), T::BUFFER_SIZE);
    return {Move(data_buffer)};
}

// ======================== END COMMUNICATION RECEIVES ========================



// ======================== BEGIN PINGS AND AVAILABILITY CHECKS ========================

bool RadioCommunication::alive() const
{
    return this->nrf24.available();
}
bool UARTCommunication::alive() const
{
    return true;  // Serial should basically always be alive.
}

bool RadioCommunication::ping()
{
    unsigned char msg[] = "PING";
    this->nrf24.write(msg, sizeof(msg));

    constexpr unsigned char resp_len = 5;
    unsigned char resp[resp_len];

    unsigned long start = millis();
    while (!this->alive())
    {
        if (millis() - start > PING_TIMEOUT_MS)
        {
            this->status = Status::RADIO_AVAILABILITY_FAILURE;
            return false;
        }
    }

    nrf24.read(resp, resp_len);
    const bool correct_response = (strncmp(reinterpret_cast<const char *>(resp), "PING", resp_len-1) == 0);

    if (!correct_response)
    {
        this->status = Status::RADIO_WRONG_HANDSHAKE_FAILURE;
        return false;
    }

    this->status = Status::NOMINAL;
    return true;
}
bool UARTCommunication::ping()
{
    const char msg[] = "PING";
    serial->print(msg);

    constexpr unsigned char resp_len = 5;
    unsigned char resp[resp_len];

    unsigned long start = millis();
    while (serial->available() < resp_len-1)  // Serial doesn't use null-terminator; total buffer should be 4-long.
    {
        if (millis() - start > PING_TIMEOUT_MS)
        {
            this->status = Status::SERIAL_AVAILABILITY_FAILURE;
            return false;
        }
    }

    serial->readBytes(resp, resp_len-1);
    resp[resp_len-1] = 0;  // Serial doesn't use null-terminator
    const bool correct_response = (strncmp(reinterpret_cast<const char *>(resp), "PING", resp_len-1) == 0);

    if (!correct_response)
    {
        this->status = Status::SERIAL_WRONG_HANDSHAKE_FAILURE;
        return false;
    }

    this->status = Status::NOMINAL;
    return true;
}

// ======================== END PINGS AND AVAILABILITY CHECKS ========================

#endif //SKIPPER_GNC_COMMUNICATION_H
