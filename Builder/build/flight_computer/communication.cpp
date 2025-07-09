#include "communication.h"
#include "constants.h"

#if defined(__AVR_ATmega328P__)       // Nano / Uno
  #include <SoftwareSerial.h>
  // on Nano, talk to Teensy over D2=RX, D3=TX
  static SoftwareSerial _softSerial(NANO_RX1_PIN, NANO_TX1_PIN);

  // this is the Nano’s “teensy_pipe”
  RTX1Communication::RTX1Communication()
    : UARTCommunication(&_softSerial)
  {}
  // we never use RTX3 on AVR, so give it a nullptr
  RTX3Communication::RTX3Communication()
    : UARTCommunication((HardwareSerial*)nullptr)
  {}

#elif defined(__IMXRT1062__)          // Teensy 4.x
  // if you ever needed an extra port for ground you could use Serial1…
  RTX1Communication::RTX1Communication()
    : UARTCommunication(&Serial1)
  {}
  // …but the flight computer→Nano link is on Serial3
  RTX3Communication::RTX3Communication()
    : UARTCommunication(&Serial3)
  {}

#else
  // all other boards, just stub out both
  RTX1Communication::RTX1Communication()
    : UARTCommunication((HardwareSerial*)nullptr)
  {}
  RTX3Communication::RTX3Communication()
    : UARTCommunication((HardwareSerial*)nullptr)
  {}
#endif

// ——— UARTCommunication constructors ———
UARTCommunication::UARTCommunication(HardwareSerial* ser)
  : serial(ser)
{
  if (ser) ser->begin(9600);
}
UARTCommunication::UARTCommunication(SoftwareSerial* ser)
  : serial(ser)
{
  ser->begin(9600);
}




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
    auto [packet, packet_len] =
        wrap_packet(
        reinterpret_cast<const uint8_t*>(encoded.get()),
        encoded_len,
        receiver_id,
        sender_id,
        command_id
        );    
    nrf24.write(packet.get(), packet_len);
}



void RadioCommunication::send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    send(data.get(), receiver_id, sender_id, command_id);
}

void UARTCommunication::send(const BaseSerializable* data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    auto [buffer, length] = data->serialize();
    size_t encoded_len;
    auto encoded = cobs_encode(buffer.get(), length, encoded_len);
    auto [packet, packet_len] =
        wrap_packet(
        reinterpret_cast<const uint8_t*>(encoded.get()),
        encoded_len,
        receiver_id,
        sender_id,
        command_id
    );    
    serial->write(packet.get(), packet_len);
}

void UARTCommunication::send(UniquePtr<BaseSerializable> data, uint8_t receiver_id, uint8_t sender_id, uint8_t command_id) {
    send(data.get(), receiver_id, sender_id, command_id);
}

UniquePtr<BaseSerializable> RadioCommunication::receive(size_t buffer_size) {
    return UniquePtr<BaseSerializable>(nullptr);
}


UniquePtr<BaseSerializable> UARTCommunication::receive(unsigned int n) {
    return receive(static_cast<size_t>(n));
}



template <typename T>
UniquePtr<T> UARTCommunication::receive() {
    while (serial->available() && serial->read() != 0xAA) ;   // wait start

    while (serial->available() < 5) ;                         // L,H,RX,TX,CMD
    uint8_t  lenL  = serial->read();
    uint8_t  lenH  = serial->read();
    uint16_t len   = (static_cast<uint16_t>(lenH) << 8) | lenL;

    uint8_t  rx_id = serial->read();
    uint8_t  tx_id = serial->read();
    uint8_t  cmd   = serial->read();
    (void)rx_id; (void)tx_id; (void)cmd;

    while (serial->available() < len - 5) ;                   // payload+CRC

    char raw[MAX_PACKET_SIZE];
    serial->readBytes(raw, len - 5);                          // pull data

    size_t   payload_len = len - 6;                           // no CRC
    uint8_t  crc_exp     = raw[payload_len];
    uint8_t  crc_act     = compute_crc8(raw, payload_len);
    if (crc_exp != crc_act) return UniquePtr<T>{nullptr};

    size_t decoded;
    auto   dec = cobs_decode(raw, payload_len, decoded);
    return UniquePtr<T>{ new T(Move(dec)) };
}

template <typename T>
UniquePtr<T> RadioCommunication::receive() {
    static_assert(T::BUFFER_SIZE + 2 <= MAX_PACKET_SIZE, "Buffer overflow risk");

    /* 1 ─ Wait for start byte */
    uint8_t start_byte;
    while (true) {
        if (!nrf24.available()) continue;
        nrf24.read(&start_byte, 1);
        if (start_byte == 0xAA) break;
    }

    /* 2 ─ Grab 5-byte header */
    while (!nrf24.available()) ;
    uint8_t hdr[5];
    nrf24.read(hdr, 5);

    uint16_t length      = static_cast<uint16_t>(hdr[0])
                         | (static_cast<uint16_t>(hdr[1]) << 8);
    uint8_t  receiver_id = hdr[2];
    uint8_t  sender_id   = hdr[3];
    uint8_t  command_id  = hdr[4];
    (void)receiver_id; (void)sender_id; (void)command_id;

    /* 3 ─ Payload + CRC */
    size_t payload_len = length - 7;          // header(6) + CRC(1)
    char   raw_payload[MAX_PACKET_SIZE];
    while (!nrf24.available()) ;
    nrf24.read(raw_payload, payload_len + 1); // payload + CRC

    /* 4 ─ CRC check */
    uint8_t expected_crc = static_cast<uint8_t>(raw_payload[payload_len]);
    uint8_t actual_crc   = compute_crc8(raw_payload, payload_len);
    if (actual_crc != expected_crc) {
        this->status = Status::RADIO_WRONG_HANDSHAKE_FAILURE;
        return UniquePtr<T>{nullptr};
    }

    /* 5 ─ Decode + deserialize */
    size_t decoded_len;
    auto   decoded_buf = cobs_decode(raw_payload, payload_len, decoded_len);
    return UniquePtr<T>{ new T(Move(decoded_buf)) };
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


template UniquePtr<Vector3>       UARTCommunication::receive<Vector3>();
template UniquePtr<IMUData>       UARTCommunication::receive<IMUData>();
template UniquePtr<StateSpace>    UARTCommunication::receive<StateSpace>();
template UniquePtr<Control>       UARTCommunication::receive<Control>();
template UniquePtr<TelemetryPacket> UARTCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket>   UARTCommunication::receive<CommandPacket>();


template UniquePtr<Vector3>       RadioCommunication::receive<Vector3>();
template UniquePtr<IMUData>       RadioCommunication::receive<IMUData>();
template UniquePtr<StateSpace>    RadioCommunication::receive<StateSpace>();
template UniquePtr<Control>       RadioCommunication::receive<Control>();
template UniquePtr<TelemetryPacket> RadioCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket>   RadioCommunication::receive<CommandPacket>();

