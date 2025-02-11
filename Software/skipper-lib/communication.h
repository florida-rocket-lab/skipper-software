//
// Created by logan on 2/7/2025.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

enum class SkipperFlag : unsigned char
{
  Idle      =     0x66,
  Armed     =     0x67, // These four correspond to the last two bits because
  Takeoff   =     0x68, // they are all mutually exclusive.
  Hover     =     0x69,
  Land      =     0x9A,
  // add more as needed.
};

struct IMUData
{

  IMUData(
    float ax=0.0, float ay=0.0, float az=0.0,
    float gx=0.0, float gy=0.0, float gz=0.0
  ): acc{ax, ay, az}, gyr{gx, gy, gz} {};

  float acc[3]; // unfiltered acceleration data in g's received from IMU6050
  float gyr[3];  // rotational velocity in deg/s received from IMU6050

};

struct SkipperState
{
  SkipperState(
    float x=0.0, float y=0.0, float z=0.0,
    float vx=0.0, float vy=0.0, float vz=0.0,
    float ax=0.0, float ay=0.0, float az=0.0,

    float rx=0.0, float ry=0.0, float rz=0.0,
    float rvx=0.0, float rvy=0.0, float rvz=0.0,
    float rax=0.0, float ray=0.0, float raz=0.0
  ): pos{x, y, z}, vel{vx, vy, vz}, acc{ax, ay, az}, rot{rx, ry, rz}, rvel{rvx, rvy, rvz}, racc{rax, ray, raz} {};

  float pos[3];  // displacement from (0, 0, 0), which is defined as position at bootup, in meters
  float vel[3];  // velocity in the *global* frame, in meters per second
  float acc[3];  // acceleration in the *global* frame, in meters per second squared

  float rot[3];  // rotation in 321 Tait-Bryan angles. "rot[0]" is defined as rotation around the x-axis in radians; "rot[1]", the y-axis; and "rot[2]", the z-axis
  float rvel[3]; // angular velocity around each axis in radians per second
  float racc[3]; // angular acceleration around each axis in radians per second squared

};

class EKF // Extended Kalman Filter
{
  public:
    EKF() {};
    SkipperState filter_data(const struct IMUData &imu_data);
  private:
    // I know there are some internal variables that need to be stored here, hence the class.
};

struct ControlSignal
{
  ControlSignal(float ga=0.0, float gb=0.0, float tcw=0.0, float tccw=0.0): gimbal_a(ga), gimbal_b(gb), thrust_cw(tcw), thrust_ccw(tccw) {};

  float gimbal_a; // intermediate float representation (i.f.r.) for top gimbal servo control signal
  float gimbal_b; // i.f.r. for bottom gimbal servo control signal
  float thrust_cw; // i.f.r. for the thrust of the clockwise-rotating portion, looking from the top down at Skipper, of the main thrust motor
  float thrust_ccw; // i.f.r. for the thrust of the counterclockwise-rotating portion, looking from the top, of the main thrust motor
};

struct Ground2Teensy
{
  Ground2Teensy(): command{} {};
  char command[32]; // 32 is a placeholder; whatever the size of our message from the UART packet requires
};

struct Teensy2Ground
{
  Teensy2Ground() = default;
  Teensy2Ground(const IMUData &id, const SkipperState &ss, const ControlSignal &cs): imu_data(id), skipper_state(ss), control_signal(cs) {};
  IMUData imu_data;
  SkipperState skipper_state;
  ControlSignal control_signal; // We're just forwarding our data to the ground station.
};

class UARTConnection
{
public:
  UARTConnection();
  void send_message(const Ground2Teensy &command);
  Teensy2Ground receive_message();
};

class RadioConnection
{
public:
  RadioConnection();
  void send_message(const Teensy2Ground &flight_data);
  Ground2Teensy receive_message();
};

#endif //COMMUNICATION_H
