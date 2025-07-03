#include "communication.h"



#if defined(__AVR_ATmega328P__)
RTX1Communication::RTX1Communication() : UARTCommunication(&Serial) {}  // Uno
#elif defined(__IMXRT1062__)
RTX1Communication::RTX1Communication() : UARTCommunication(&Serial1) {}  // Teensy
#else
RTX1Communication::RTX1Communication() : UARTCommunication(nullptr) {}
#endif


RadioCommunication::RadioCommunication(unsigned char ce_pin, unsigned char csn_pin, const uint8_t address_[6], unsigned char rf_channel_)
    : BaseCommunication(),
      nrf24(ce_pin, csn_pin),
      address(address_),
      rf_channel(rf_channel_)
{
    // Intentionally left blank - call init() manually after Serial.begin()
}

// void RadioCommunication::init() {
//     if (!nrf24.begin()) {
//         status = Status::RADIO_INIT_FAILURE;
//         return;
//     }
//     nrf24.setChannel(rf_channel);
//     nrf24.setDataRate(RF24_250KBPS);
//     nrf24.setPALevel(RF24_PA_LOW);
//     nrf24.openWritingPipe(address);
//     nrf24.openReadingPipe(1, address);
//     nrf24.stopListening();
// }
void RadioCommunication::init() {
    Serial.println("INIT: Starting radio...");
    if (!nrf24.begin()) {
        Serial.println("INIT: Radio failed to begin");
        status = Status::RADIO_INIT_FAILURE;
        return;
    }
    Serial.println("INIT: Radio began");

    nrf24.setChannel(rf_channel);
    Serial.println("INIT: Set channel");

    nrf24.setDataRate(RF24_250KBPS);
    Serial.println("INIT: Set data rate");

    nrf24.setPALevel(RF24_PA_LOW);
    Serial.println("INIT: Set PA level");

    nrf24.openWritingPipe(address);
    Serial.println("INIT: Opened writing pipe");

    nrf24.openReadingPipe(1, address);
    Serial.println("INIT: Opened reading pipe");

    nrf24.stopListening();
    Serial.println("INIT: Ready to transmit");

    status = Status::OK;
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

bool RadioCommunication::alive() const {
    return nrf24.available();
}
bool UARTCommunication::alive() const {
    return true;
}

void RadioCommunication::send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    auto [buffer, length] = data->serialize();

    size_t encoded_len;

    auto encoded = cobs_encode(buffer.get(), length, encoded_len);
    auto [packet, packet_len] = wrap_packet(encoded.get(), encoded_len, receiver_id, sender_id, command_id);
    nrf24.write(packet.get(), packet_len);
}



void RadioCommunication::send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    send(data.get(), receiver_id, sender_id, command_id);
}

void UARTCommunication::send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    auto [buffer, length] = data->serialize();
    size_t encoded_len;
    auto encoded = cobs_encode(buffer.get(), length, encoded_len);
    auto [packet, packet_len] = wrap_packet(encoded.get(), encoded_len, receiver_id, sender_id, command_id);
    serial->write(packet.get(), packet_len);
}

void UARTCommunication::send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    send(data.get(), receiver_id, sender_id, command_id);
}

UniquePtr<BaseSerializable> RadioCommunication::receive(size_t buffer_size) {
    return UniquePtr<BaseSerializable>(nullptr);
}



template <typename T>
UniquePtr<T> UARTCommunication::receive() {
    static_assert(T::BUFFER_SIZE + 2 <= MAX_PACKET_SIZE, "Buffer overflow risk");

    // 1. Wait for start byte
    while (true) {
        if (serial->available() && serial->read() == 0xAA) break;
    }

    // 2. Read header (length, receiver_id, sender_id, command_id)
    while (serial->available() < 4); // wait until header is ready

    uint8_t length = serial->read(); // total length
    uint8_t receiver_id = serial->read();
    uint8_t sender_id = serial->read();
    uint8_t command_id = serial->read();

    // 3. Wait for payload + CRC
    while (serial->available() < length - 5); // -5 = 1 (start byte already read) + 4 header bytes

    char raw_payload[MAX_PACKET_SIZE];
    serial->readBytes(raw_payload, length - 5);

    // 4. Split payload and CRC
    size_t encoded_len = length - 6;  // total - header (5) - crc (1)
    char* payload_data = raw_payload;
    uint8_t expected_crc = static_cast<uint8_t>(raw_payload[encoded_len]);

    // 5. Validate CRC
    uint8_t actual_crc = compute_crc8(payload_data, encoded_len);
    if (actual_crc != expected_crc) {
        this->status = Status::SERIAL_WRONG_HANDSHAKE_FAILURE;
        return UniquePtr<T>{nullptr};
    }

    // 6. Decode + Deserialize
    size_t decoded_len;
    auto decoded_buf = cobs_decode(payload_data, encoded_len, decoded_len);
    auto obj = new T(Move(decoded_buf));
    return UniquePtr<T>{obj};
}

UniquePtr<BaseSerializable> UARTCommunication::receive(size_t) {
    return UniquePtr<BaseSerializable>(nullptr);
}

template <typename T>
UniquePtr<T> RadioCommunication::receive() {
    static_assert(T::BUFFER_SIZE + 2 <= MAX_PACKET_SIZE, "Buffer overflow risk");


    // 1. Wait for start byte 0xAA
    uint8_t start_byte;
    while (true) {
        if (!nrf24.available()) continue;

        nrf24.read(&start_byte, 1);
        if (start_byte == 0xAA) break;
    }

    // 2. Wait for rest of the header (4 bytes: length, receiver_id, sender_id, command_id)
    while (!nrf24.available()) ;
    uint8_t header[4];
    nrf24.read(header, 4);
    uint8_t length      = header[0];
    uint8_t receiver_id = header[1];
    uint8_t sender_id   = header[2];
    uint8_t command_id  = header[3];

    (void)receiver_id;
    (void)sender_id;
    (void)command_id;

    // 3. Wait for payload + CRC
    size_t payload_len = length - 6;  // total - start - 4 header - crc
    char raw_payload[MAX_PACKET_SIZE];
    while (!nrf24.available()) ;
    nrf24.read(raw_payload, payload_len + 1); // payload + crc

    // 4. CRC check
    uint8_t expected_crc = static_cast<uint8_t>(raw_payload[payload_len]);
    uint8_t actual_crc   = compute_crc8(raw_payload, payload_len);
    if (actual_crc != expected_crc) {
        this->status = Status::RADIO_WRONG_HANDSHAKE_FAILURE;
        return UniquePtr<T>{nullptr};
    }

    // 5. Decode + Deserialize
    size_t decoded_len;
    auto decoded_buf = cobs_decode(raw_payload, payload_len, decoded_len);
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


template UniquePtr<Vector3>       RadioCommunication::receive<Vector3>();
template UniquePtr<IMUData>       RadioCommunication::receive<IMUData>();
template UniquePtr<StateSpace>    RadioCommunication::receive<StateSpace>();
template UniquePtr<Control>       RadioCommunication::receive<Control>();
template UniquePtr<TelemetryPacket> RadioCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket> RadioCommunication::receive<CommandPacket>();