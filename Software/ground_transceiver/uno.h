//
// Created by logan on 3/6/2025.
//

#ifndef SKIPPER_GNC_UNO_H
#define SKIPPER_GNC_UNO_H

#include "../shared/skipper_lib.h"

class Uno
{
public:
    // TODO: ADD SHIT.

private:
    USBCommunication<TelemetryPacket> to_terminal;
    USBCommunication<StringPacket<MESSAGE_SIZE>> from_terminal;
    RadioCommunication<StringPacket<MESSAGE_SIZE>> to_nano;
    RadioCommunication<TelemetryPacket> from_nano;
};


#endif //SKIPPER_GNC_UNO_H
