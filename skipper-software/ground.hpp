
#ifndef GROUND_H
#define GROUND_H

#include "teensy.hpp"
#include "nano.hpp"

class Ground
{
    public:
        Ground();
        void communication_loop() const;
    private:
        RadioConnection _with_ground;
        struct Teensy2Ground _received_flight_data;
        struct Ground2Teensy _command_data_buffer;
        void terminal_to_sky() const;
        void sky_to_terminal() const;
};

#endif // GROUND_H
