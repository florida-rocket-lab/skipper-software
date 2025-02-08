#include "teensy.h"
#include "../skipper-lib/_wrapper.h"

IMUData get_imu_data()
{
  // TODO: retrieve IMU Data from a library/Drehmflight?
  return {};
}

ControlSignal run_GNC(const SkipperState& state, const SkipperFlag flags=SkipperFlag::Idle)
{
  // TODO: take the state struct and run GNC's control model to produce the control signals
  switch (flags)
  {
    case SkipperFlag::Idle:
      // run this GNC section whenever Skipper is doing absolutely nothing
      return {}; // this return might be fine just as it is
    case SkipperFlag::Armed:
      // run this GNC section whenever Skipper is on the ground getting ready to fly
      return {}; // this return definitely must change
    case SkipperFlag::Takeoff:
      // run this GNC section whenever Skipper is actively lifting up off the ground
      return {}; // this return definitely must change
    case SkipperFlag::Hover:
      // run this GNC section whenever Skipper is actively hovering
      return {}; // this return definitely must change
  }
}

void update_controls(const ControlSignal& control_signal)
{
  // TODO: take the floats and convert them to the actual signal that needs to get sent to the physical components
  // TODO: send the actual data to the components
}

Ground2Teensy read_message_from_nano()
{
  // TODO: set the connecting pin from the Teensy 4.0 to the Nano to HI, letting the Nano know that we're good to read.
  // TODO: read the streaming UART data from Nano
  // TODO: set the connecting pin from the Teensy 4.0 to the Nano to LOW, letting the Nano know to get ready for our message.
  return {};
}

void update_skipper_flags(const Ground2Teensy &command, enum class SkipperFlag &skipper_flag)
{
  // TODO: implement the command logic with skipper flag
  // if (command == "goto IDLE") skipper_flag = SkipperFlag::Idle;
  // if (command == "goto ARMED") skipper_flag = SkipperFlag::Armed;
  // if (command == "goto TAKEOFF") skipper_flag = SkipperFlag::Takeoff;
  // if (STOP_TAKEOFF?) skipper_flag = SkipperFlag::Hover;
  // if (command == "goto HOVER") skipper_flag = SkipperFlag::Hover;
}

void send_message_to_nano(const IMUData &id, const SkipperState &ss, const ControlSignal &cs)
{
  Teensy2Ground t2g {id, ss, cs};
  // TODO: serialize the data? or do something to prepare the data to be sent to the Nano.
  // TODO: send the data to the Nano via UART
}

IMUData imu_data {};
SkipperState skipper_state {};
EKF ekf {};
ControlSignal control_signal {};
Ground2Teensy command {};
SkipperFlag skipper_flag = SkipperFlag::Idle;

void setup() 
{
  // I don't know what needs to be initialized. Is there an initial UART handshake?
}

void loop() 
{
  imu_data = get_imu_data();
  skipper_state = ekf.filter_data(imu_data);
  control_signal = run_GNC(skipper_state, skipper_flag);
  update_controls(control_signal);
  command = read_message_from_nano();
  update_skipper_flags(command, skipper_flag);
  send_message_to_nano(imu_data, skipper_state, control_signal);
}
