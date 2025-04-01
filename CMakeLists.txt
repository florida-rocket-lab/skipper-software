# This is merely to trick CLion into providing code insight features.

cmake_minimum_required(VERSION 3.10)
project(Skipper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(unit_test
        Software/tests/test_suite.cpp
        Software/catch2/catch_amalgamated.cpp
        Software/flight_computer/flight_computer.ino
        Software/flight_computer/teensy.cpp
        Software/ground_transceiver/ground_transceiver.ino
        Software/ground_transceiver/uno.cpp
        Software/skipper_transceiver/skipper_transceiver.ino
        Software/skipper_transceiver/nano.cpp
        Software/shared/communication.cpp
        Software/shared/datatypes.cpp
)
