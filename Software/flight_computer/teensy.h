//
// Created by logan on 3/5/2025.
//

#ifndef SKIPPER_GNC_TEENSY_H
#define SKIPPER_GNC_TEENSY_H

#include "../shared/skipper_lib.h"

class Teensy
{
public:
    // TODO: ADD SHIT.

private:
    RTX3Communication<TelemetryPacket> to_nano;
    RTX3Communication<StringPacket<MESSAGE_SIZE>> from_nano;
};


#endif //SKIPPER_GNC_TEENSY_H
