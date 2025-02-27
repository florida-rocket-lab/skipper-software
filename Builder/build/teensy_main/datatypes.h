
#ifndef _DATATYPES_H
#define _DATATYPES_H


#include <stdint.h>  
#include <string.h> 


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
  char command[32]; // 32 is a placeholder;
  
  void serialize(uint8_t* buffer) const {
    memcpy(buffer, command, sizeof(command)); // copy command into buffer
}

static bool deserialize(const uint8_t* buffer, char* out_command) {
    memcpy(out_command, buffer, 32);  // copy back into command
    return true; 
}

};

struct Teensy2Ground
{
  Teensy2Ground() = default;
  Teensy2Ground(const IMUData &id, const SkipperState &ss, const ControlSignal &cs): imu_data(id), skipper_state(ss), control_signal(cs) {};
  IMUData imu_data;
  SkipperState skipper_state;
  ControlSignal control_signal; // We're just forwarding our data to the ground station.

  void serialize(uint8_t* buffer) const {
    memcpy(buffer, this, sizeof(Teensy2Ground));  // Copy full struct
  }

  bool deserialize(const uint8_t* buffer) {
    memcpy(this, buffer, sizeof(Teensy2Ground));
    return true;
  }

};

#endif //_DATATYPES_H
