//
// Created by logan on 3/6/2025.
//

#ifndef SKIPPER_GNC_NANO_H
#define SKIPPER_GNC_NANO_H

#include "../shared/skipper_lib.h"

class Nano
{
public:
    // TODO: ADD SHIT.

private:
    RTX1Communication<TelemetryPacket> from_teensy;
    RTX1Communication<StringPacket<MESSAGE_SIZE>> to_teensy;
    RadioCommunication<TelemetryPacket> to_uno;
    RadioCommunication<StringPacket<MESSAGE_SIZE>> from_uno;
};


#endif //SKIPPER_GNC_NANO_H
