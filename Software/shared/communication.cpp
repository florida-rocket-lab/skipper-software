#include "communication.h"
#include <string.h>


static uint8_t RADIO_RX_BUF[MAX_PACKET_SIZE];
static char UART_RX_BUF[MAX_PACKET_SIZE];
uint8_t TX_BUF[MAX_PACKET_SIZE];  
uint8_t RX_BUF[MAX_PACKET_SIZE]; 



RadioCommunication::RadioCommunication(uint8_t ce, uint8_t csn,
                                       const uint8_t addr[6],
                                       uint8_t channel)
  : nrf(ce, csn),
    address(addr),
    rf_channel(channel)
{}

void RadioCommunication::init() {
    if (!nrf.begin()) {
        status = Status::RADIO_INIT_FAILURE;
        return;
    }
    nrf.enableDynamicPayloads();
    nrf.setPayloadSize(32);     
    nrf.startListening();

    nrf.setChannel(rf_channel);
    nrf.setDataRate(RF24_250KBPS);
    nrf.setPALevel(RF24_PA_LOW);
    nrf.openWritingPipe(address);
    nrf.openReadingPipe(1, address);
    nrf.startListening(); 
    status = Status::OK;
}

bool RadioCommunication::alive() {
    return nrf.available();
}

bool RadioCommunication::ping() {
    const char pingMsg[] = "PING";
    nrf.stopListening();
    nrf.write(pingMsg, sizeof(pingMsg));
    nrf.startListening();
    unsigned long start = millis();
    while (!alive()) {
        if (millis() - start > PING_TIMEOUT_MS) {
            status = Status::RADIO_AVAILABILITY_FAILURE;
            return false;
        }
    }
    char resp[4];
    nrf.read(resp, 4);
    if (strncmp(resp, "PING", 4) != 0) {
        status = Status::RADIO_WRONG_HANDSHAKE_FAILURE;
        return false;
    }
    status = Status::OK;
    return true;
}

void RadioCommunication::send(const BaseSerializable* d, uint8_t rx, uint8_t tx, uint8_t cmd) {
    auto [buf, len] = d->serialize();
    size_t cobs_len;
    auto enc = cobs_encode(buf.get(), len, cobs_len);
    auto [pkt, pkt_len] = wrap_packet(
      reinterpret_cast<const uint8_t*>(enc.get()),
      cobs_len, rx, tx, cmd
    );
    for (size_t off = 0; off < pkt_len; off += 32) {
        size_t chunk = Min<size_t>(32, pkt_len - off);
        nrf.write(pkt.get() + off, chunk);   
    }
    nrf.startListening();
}

void RadioCommunication::send(UniquePtr<BaseSerializable> d, uint8_t rx, uint8_t tx, uint8_t cmd) {
    send(d.get(), rx, tx, cmd);
}

UniquePtr<BaseSerializable> RadioCommunication::receive(size_t) {
    return {};
}

template<typename T>
UniquePtr<T> RadioCommunication::receive() {
    static_assert(T::BUFFER_SIZE + 2 <= MAX_PACKET_SIZE, "Buffer overflow");
    // 1) wait start
    uint8_t b;
    unsigned long t0 = millis();
    for (;;) {
        if (nrf.available()) {
            nrf.read(&b, 1);
            if (b == 0xAA) break;
        }
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::RADIO_AVAILABILITY_FAILURE;
            return {};
        }
    }
    
    // 2) header
    t0 = millis();
    while (!nrf.available()) {
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::RADIO_AVAILABILITY_FAILURE;
            return {};
        }
    }
    uint8_t hdr[6];                 // one extra byte for LEN MSB
    nrf.read(hdr, 6);
    uint16_t len = uint16_t(hdr[1]) | (uint16_t(hdr[2]) << 8);
    // 3) payload+CRC
    auto& raw = RADIO_RX_BUF;
    t0 = millis();
    while (!nrf.available()) {
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::RADIO_AVAILABILITY_FAILURE;
            return {};
        }
    }   
    nrf.read(raw, len - 2);
    // 4) CRC
    size_t pl = (len - 2) - 1; 
    if (compute_crc8(reinterpret_cast<const char*>(raw), pl) != raw[pl]) {
        return {};
    }
    // 5) COBS decode + construct
    size_t dec_len;
    UniquePtr<char[]> dec = cobs_decode(reinterpret_cast<const char*>(raw), pl, dec_len);
    return UniquePtr<T>( new T(Move(dec)) );

}

template UniquePtr<Vector3>  RadioCommunication::receive<Vector3>();
template UniquePtr<IMUData>  RadioCommunication::receive<IMUData>();

//  UARTCommunication 

UARTCommunication::UARTCommunication(Stream* ser)
  : serial(ser)
{
    if (!serial) status = Status::SERIAL_INIT_FAILURE;
}

bool UARTCommunication::alive() {
    return serial->available() > 0;
}

bool UARTCommunication::ping() {
    const char msg[] = "PING";
    serial->print(msg);
    unsigned long start = millis();
    while (serial->available() < 4) {
        if (millis() - start > PING_TIMEOUT_MS) {
            status = Status::SERIAL_AVAILABILITY_FAILURE;
            return false;
        }
    }
    char resp[5] = {0};
    serial->readBytes(resp, 4);
    if (strncmp(resp, "PING", 4) != 0) {
        status = Status::SERIAL_WRONG_HANDSHAKE_FAILURE;
        return false;
    }
    status = Status::OK;
    return true;
}

void UARTCommunication::send(const BaseSerializable* d, uint8_t rx, uint8_t tx, uint8_t cmd) {
    auto [buf, len] = d->serialize();
    size_t cobs_len;
    auto enc = cobs_encode(buf.get(), len, cobs_len);
    auto [pkt, pkt_len] = wrap_packet(
      reinterpret_cast<const uint8_t*>(enc.get()),
      cobs_len, rx, tx, cmd
    );
    serial->write(pkt.get(), pkt_len);
}

void UARTCommunication::send(UniquePtr<BaseSerializable> d, uint8_t rx, uint8_t tx, uint8_t cmd) {
    send(d.get(), rx, tx, cmd);
}

UniquePtr<BaseSerializable> UARTCommunication::receive(size_t) {
    return {};
}

template<typename T>
UniquePtr<T> UARTCommunication::receive() {
    // 1) skip until 0xAA
    unsigned long t0 = millis();
    for (;;) {
        if (serial->available() && serial->read() == 0xAA) break;
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::SERIAL_AVAILABILITY_FAILURE;
            return {};
        }
    }
    // 2) header
    t0 = millis();
    while (serial->available() < 5) {
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::SERIAL_AVAILABILITY_FAILURE;
            return {};
        }
    }
    uint8_t hdr[5];                     
    serial->readBytes(hdr, 5);                
    uint16_t len = uint16_t(hdr[0]) | (uint16_t(hdr[1]) << 8);

    uint8_t  rx  = hdr[2];
    uint8_t  tx  = hdr[3];
    uint8_t  cmd = hdr[4];
    size_t toRead = len - 2; 




    auto& raw = UART_RX_BUF;
    t0 = millis();
    while (serial->available() < (int)toRead) {
        if (millis() - t0 > PING_TIMEOUT_MS) {
            status = Status::SERIAL_AVAILABILITY_FAILURE;
            return {};
        }
    }
    serial->readBytes(raw, toRead);
    size_t pl = toRead - 1;
    Serial.print(F("CRC calc=")); Serial.print(compute_crc8(raw, pl), HEX);
    Serial.print(F("  CRC rx=")); Serial.println(raw[pl], HEX);
    if (compute_crc8(raw, pl) != raw[pl]) { 
        return {};
    }
    // 4) COBS decode + construct
    size_t dec_len;
    UniquePtr<char[]> dec = cobs_decode(raw, pl, dec_len);
    return UniquePtr<T>( new T(Move(dec)) );
}

// Explicit instantiations:
template UniquePtr<Vector3>      UARTCommunication::receive<Vector3>();
template UniquePtr<IMUData>      UARTCommunication::receive<IMUData>();

template UniquePtr<StateSpace>       RadioCommunication::receive<StateSpace>();
template UniquePtr<Control>          RadioCommunication::receive<Control>();
template UniquePtr<TelemetryPacket>  RadioCommunication::receive<TelemetryPacket>();
template UniquePtr<CommandPacket>    RadioCommunication::receive<CommandPacket>();
template UniquePtr<TelemetryPacket> UARTCommunication::receive<TelemetryPacket>();



USBCommunication::USBCommunication()
  : UARTCommunication(&Serial)
{}

RTX1Communication::RTX1Communication()
  : UARTCommunication(&Serial) { }

RTX3Communication::RTX3Communication()
  : UARTCommunication(&Serial) { }
