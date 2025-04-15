#include "communication.h"

RadioCommunication::RadioCommunication(unsigned char ce_pin, unsigned char csn_pin, const uint8_t address[6], unsigned char rf_channel)
    : BaseCommunication(), nrf24(ce_pin, csn_pin)
{
    if (!nrf24.begin()) {
        this->status = Status::RADIO_INIT_FAILURE;
        return;
    }
    nrf24.setChannel(rf_channel);
    nrf24.setDataRate(RF24_250KBPS);
    nrf24.setPALevel(RF24_PA_LOW);
    nrf24.openWritingPipe(address);
    nrf24.openReadingPipe(1, address);
    nrf24.stopListening();
}

UARTCommunication::UARTCommunication(HardwareSerial* serial_) : serial(serial_)
{
    serial->begin(9600);
    if (!(*serial)) {
        this->status = Status::SERIAL_INIT_FAILURE;
    }
}

USBCommunication::USBCommunication()
#if defined(__AVR_ATmega328P__)
    : UARTCommunication(&Serial)
#elif defined(__IMXRT1062__)
    : UARTCommunication(&Serial1)
#else
    : UARTCommunication(nullptr)
#endif
{}

RTX1Communication::RTX1Communication() : UARTCommunication(&Serial1) {}
RTX3Communication::RTX3Communication() : UARTCommunication(&Serial3) {}

bool RadioCommunication::alive() const {
    return nrf24.available();
}
bool UARTCommunication::alive() const {
    return true;
}

void RadioCommunication::send(const BaseSerializable* data) {
    auto [buffer, length] = data->serialize();
    size_t encoded_len;
    auto encoded = cobs_encode(buffer.get(), length, encoded_len);
    nrf24.write(encoded.get(), encoded_len);
}

void RadioCommunication::send(UniquePtr<BaseSerializable> data) {
    send(data.get());
}

void UARTCommunication::send(const BaseSerializable* data) {
    auto [buffer, length] = data->serialize();
    size_t encoded_len;
    auto encoded = cobs_encode(buffer.get(), length, encoded_len);
    serial->write(encoded.get(), encoded_len);
}

void UARTCommunication::send(UniquePtr<BaseSerializable> data) {
    send(data.get());
}

UniquePtr<BaseSerializable> UARTCommunication::receive(size_t) {
    return UniquePtr<BaseSerializable>{nullptr};
}

template <typename T>
UniquePtr<T> UARTCommunication::receive() {

    static_assert(T::BUFFER_SIZE + 2 <= MAX_PACKET_SIZE, "Buffer overflow risk");

    char raw_buf[MAX_PACKET_SIZE];
    size_t len = T::BUFFER_SIZE + 2;

    unsigned long start = millis();
    while ((size_t)serial->available() < len) {
        if (millis() - start > MESSAGE_TIMEOUT_MS) {
            this->status = Status::SERIAL_AVAILABILITY_FAILURE;
            return UniquePtr<T>{nullptr};
        }
    }

    serial->readBytes(raw_buf, len);

    size_t decoded_len;
    auto decoded_buf = cobs_decode(raw_buf, len, decoded_len);
    auto obj = new T(Move(decoded_buf));
    return UniquePtr<T>{obj};
}

bool RadioCommunication::ping() {
    unsigned char msg[] = "PING";
    nrf24.write(msg, sizeof(msg));

    unsigned char resp[5];
    unsigned long start = millis();
    while (!alive()) {
        if (millis() - start > PING_TIMEOUT_MS) {
            status = Status::RADIO_AVAILABILITY_FAILURE;
            return false;
        }
    }

    nrf24.read(resp, 5);
    if (strncmp((char*)resp, "PING", 4) != 0) {
        status = Status::RADIO_WRONG_HANDSHAKE_FAILURE;
        return false;
    }

    status = Status::NOMINAL;
    return true;
}

bool UARTCommunication::ping() {
    const char msg[] = "PING";
    serial->print(msg);

    unsigned char resp[5];
    unsigned long start = millis();
    while (serial->available() < 4) {
        if (millis() - start > PING_TIMEOUT_MS) {
            status = Status::SERIAL_AVAILABILITY_FAILURE;
            return false;
        }
    }

    serial->readBytes(resp, 4);
    resp[4] = 0;

    if (strncmp((char*)resp, "PING", 4) != 0) {
        status = Status::SERIAL_WRONG_HANDSHAKE_FAILURE;
        return false;
    }

    status = Status::NOMINAL;
    return true;
}
