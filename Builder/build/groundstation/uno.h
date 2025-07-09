#ifndef SKIPPER_GNC_UNO_H
#define SKIPPER_GNC_UNO_H

#include "skipper_lib.h"

class Uno {
public:
    Uno();

    /// Bring up the RF link to the Nano
    void init_radio();

    /// Listen for a TelemetryPacket and print it over USB
    void receive_from_sky();

private:
    // Must match the Nano’s address
    static const uint8_t RADIO_ADDRESS[6];

    // Buffer to hold the last-received telemetry (nullptr if none)
    UniquePtr<TelemetryPacket> telemetry_buffer;

    // RF24 link to the Nano
    RadioCommunication nano_pipe;
};

#endif // SKIPPER_GNC_UNO_H
