#ifndef SKIPPER_GNC_COMMUNICATION_H
#define SKIPPER_GNC_COMMUNICATION_H

#include <cstring>
#include <utility>
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

template <typename T>  // It's a bit goofy, but you can't have templated virtual functions.
class BaseCommunication
{
public:
    virtual bool ping() = 0;  // Handshake to see if the connection is live.
    [[nodiscard]] virtual bool alive() const = 0;  // Check to see if the connection is still available.

    virtual void send(const T& data) = 0;  // Every communication protocol needs to be able to send data.
    virtual T receive() = 0;  // Every communication protocol needs to receive and **deserialize** the data.

    Status get_status() {return this->_status; };

protected:
    void set_status(Status new_status) {this->_status = new_status; };

private:
    Status _status = Status::NOMINAL;
};

template <typename T>
class RadioCommunication: virtual public BaseCommunication<T>
{
public:
    RadioCommunication(unsigned char ce_pin, unsigned char csn_pin, const char address[6] = RADIO_COMMUNICATION_ADDRESS, unsigned char rf_channel = 108);

    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const T& data) override;
    T receive() override;

private:
    RF24 nrf24;
};

template <typename T>
class UARTCommunication: virtual public BaseCommunication<T>
{
public:
    explicit UARTCommunication(HardwareSerial* serial);

    bool ping() override;
    [[nodiscard]] bool alive() const override;

    void send(const T& data) override;
    T receive() override;

private:
    HardwareSerial* _serial;
};

template <typename T>
class USBCommunication: virtual public UARTCommunication<T>
{
    USBCommunication(): UARTCommunication<T>(Serial) {};
};

template <typename T>
class RTX1Communication: virtual public UARTCommunication<T>
{
    RTX1Communication(): UARTCommunication<T>(Serial1) {};
};

template <typename T>
class RTX3Communication: virtual public UARTCommunication<T>
{
    RTX3Communication(): UARTCommunication<T>(Serial3) {};
};


// ======================== BEGIN RADIO COMMUNICATION CODE ========================

template <typename T>
RadioCommunication<T>::RadioCommunication(const unsigned char ce_pin, const unsigned char csn_pin, const char address[6], const unsigned char rf_channel): nrf24(ce_pin, csn_pin)
{
    if (!nrf24.begin())
    {
        this->set_status(Status::RADIO_INIT_FAILURE);
        return;
    }

    nrf24.setChannel(rf_channel);  // Straight up ripped from Jose Reynaldo's original implementation.
    nrf24.setDataRate(RF24_250KBPS); // possible values: RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
    nrf24.setPALevel(RF24_PA_LOW); // possible values: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
    nrf24.openWritingPipe(address); // writing pipe address
    nrf24.openReadingPipe(1, address); // reading pipe address
    nrf24.stopListening(); // default as transmitter
}
template <typename T>
bool RadioCommunication<T>::alive() const
{
    return this->nrf24.available();
}
template <typename T>
bool RadioCommunication<T>::ping()
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
            this->set_status(Status::RADIO_AVAILABILITY_FAILURE);
            return false;
        }
    }

    nrf24.read(resp, resp_len);
    const bool correct_response = (std::strncmp(reinterpret_cast<const char *>(resp), "PING", resp_len-1) == 0);

    if (!correct_response)
    {
        this->set_status(Status::RADIO_WRONG_HANDSHAKE_FAILURE);
        return false;
    }
    return true;
}
template <typename T>
void RadioCommunication<T>::send(const T& data)
{
    std::pair<std::unique_ptr<char[]>, unsigned int> data_buffer = data.serialize();
    if (!this->alive())
    {
        this->set_status(Status::RADIO_AVAILABILITY_FAILURE);
        return;
    }
    nrf24.write(data_buffer.first.get(), data_buffer.second);
    nrf24.available();
}
template <typename T>
T RadioCommunication<T>::receive()
{
    std::unique_ptr<char[]> data_buffer = std::make_unique<char[]>(T::BUFFER_SIZE);
    if (!this->alive())
    {
        this->set_status(Status::RADIO_AVAILABILITY_FAILURE);
        return {};
    }
    nrf24.read(data_buffer.get(), T::BUFFER_SIZE);
    return {data_buffer};
}

// ======================== END RADIO COMMUNICATION CODE ========================

// ======================== BEGIN UART COMMUNICATION CODE ========================

template <typename T>
UARTCommunication<T>::UARTCommunication(HardwareSerial* serial): _serial(serial)
{
    _serial->begin(9600);
    if (!(*_serial))
    {
        this->set_status(Status::SERIAL_INIT_FAILURE);
        return;
    }
}
template <typename T>
bool UARTCommunication<T>::alive() const
{
    return true;  // Serial should basically always be alive.
}
template <typename T>
bool UARTCommunication<T>::ping()
{
    unsigned char msg[] = "PING";
    _serial->print(msg);

    constexpr unsigned char resp_len = 5;
    unsigned char resp[resp_len];

    unsigned long start = millis();
    while (_serial->available() < resp_len-1)  // Serial doesn't use null-terminator; total buffer should be 4-long.
    {
        if (millis() - start > PING_TIMEOUT_MS)
        {
            this->set_status(Status::SERIAL_AVAILABILITY_FAILURE);
            return false;
        }
    }

    _serial->readBytes(resp, resp_len-1);
    resp[resp_len-1] = 0;  // Serial doesn't use null-terminator
    const bool correct_response = (std::strncmp(reinterpret_cast<const char *>(resp), "PING", resp_len-1) == 0);

    if (!correct_response)
    {
        this->set_status(Status::SERIAL_WRONG_HANDSHAKE_FAILURE);
        return false;
    }
    return true;
}
template <typename T>
void UARTCommunication<T>::send(const T& data)
{
    std::pair<std::unique_ptr<char[]>, unsigned int> data_buffer = data.serialize();
    _serial->write(data_buffer.first.get(), data_buffer.second);
}
template <typename T>
T UARTCommunication<T>::receive()
{
    std::unique_ptr<char[]> data_buffer = std::make_unique<char[]>(T::BUFFER_SIZE);

    unsigned long start = millis();
    while (_serial->available() < T::BUFFER_SIZE)  // Serial doesn't use null-terminator; total buffer should be 4-long.
    {
        if (millis() - start > MESSAGE_TIMEOUT_MS)
        {
            this->set_status(Status::SERIAL_AVAILABILITY_FAILURE);
            return {};
        }
    }

    _serial->readBytes(data_buffer.get(), T::BUFFER_SIZE);
    return {data_buffer};
}
// ======================== END UART COMMUNICATION CODE ========================

#endif //SKIPPER_GNC_COMMUNICATION_H
