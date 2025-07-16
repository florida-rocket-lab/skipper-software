#ifndef SKIPPER_LQI_STUB_H
#define SKIPPER_LQI_STUB_H

// mimic the RTM struct that your teensy.cpp expects
struct SkipperLqiOutput {
  float thrust;
  float counter_rotating;
  float upper_gimbal_angle;
  float lower_gimbal_angle;
};
struct RT_MODEL_skipper_lqi_T {
  SkipperLqiOutput skipper_lqi_Y;
};


class skipper_lqi {
public:
  void initialize() { /* no-op */ }
  void step()      { /* no-op */ }
  RT_MODEL_skipper_lqi_T* getRTM() { return &rtm; }
private:
  RT_MODEL_skipper_lqi_T rtm{};
};

#endif
