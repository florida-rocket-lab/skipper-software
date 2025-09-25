//
// Sponsored License - for use in support of a program or activity
// sponsored by MathWorks.  Not for government, commercial or other
// non-sponsored organizational use.
//
// File: skipper_lqi_for_export.cpp
//
// Code generated for Simulink model 'skipper_lqi_for_export'.
//
// Model version                  : 1.123
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Sep 25 17:17:21 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "skipper_lqi_for_export.h"
#include <cstring>
#include "rtwtypes.h"
#include "limits"
#include "cmath"

extern "C"
{
  real_T rtNaN { -std::numeric_limits<real_T>::quiet_NaN() };

  real_T rtInf { std::numeric_limits<real_T>::infinity() };

  real_T rtMinusInf { -std::numeric_limits<real_T>::infinity() };

  real32_T rtNaNF { -std::numeric_limits<real32_T>::quiet_NaN() };

  real32_T rtInfF { std::numeric_limits<real32_T>::infinity() };

  real32_T rtMinusInfF { -std::numeric_limits<real32_T>::infinity() };
}

// Model step function
void skipperLQR::step()
{
  real_T state_with_error[15];
  real_T state_with_error_tmp[15];
  real_T tmp_0[15];
  real_T tmp_1[15];
  real_T tmp_2[15];
  real_T rtb_passthrough_gain[12];
  real_T tmp[12];
  real_T rtb_control[4];
  real_T rtb_passthrough_gain_0;
  real_T tmp_3;
  real_T tmp_4;
  real_T tmp_5;
  real_T tmp_6;
  real_T tmp_7;
  real_T tmp_8;
  real_T tmp_9;
  real_T tmp_a;
  int32_T i;
  int32_T i_0;
  for (i = 0; i < 12; i++) {
    // Sum: '<Root>/feedback_sum' incorporates:
    //   Gain: '<Root>/passthrough_gain'
    //   UnitDelay: '<Root>/Unit Delay'

    rtb_passthrough_gain[i] = rtDW.UnitDelay_DSTATE[i];

    // Gain: '<Root>/state_gain'
    tmp[i] = 0.0;
  }

  // Gain: '<Root>/extraction_gain'
  tmp_8 = 0.0;
  tmp_9 = 0.0;
  tmp_a = 0.0;
  for (i = 0; i < 12; i++) {
    // Gain: '<Root>/state_gain' incorporates:
    //   Gain: '<Root>/passthrough_gain'

    rtb_passthrough_gain_0 = rtb_passthrough_gain[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      tmp[i_0] += rtConstP.state_gain_Gain[12 * i + i_0] *
        rtb_passthrough_gain_0;
    }

    // Gain: '<Root>/extraction_gain' incorporates:
    //   Gain: '<Root>/passthrough_gain'

    tmp_8 += rtConstP.extraction_gain_Gain[3 * i] * rtb_passthrough_gain_0;
    tmp_9 += rtConstP.extraction_gain_Gain[3 * i + 1] * rtb_passthrough_gain_0;
    tmp_a += rtConstP.extraction_gain_Gain[3 * i + 2] * rtb_passthrough_gain_0;

    // Sum: '<Root>/state_derivative_sum' incorporates:
    //   Constant: '<Root>/gravity_const'

    tmp_0[i] = rtConstP.gravity_const_Value[i];
  }

  // Sum: '<Root>/state_derivative_sum' incorporates:
  //   Constant: '<Root>/dummy_zeros'
  //   Inport: '<Root>/refeference_in'
  //   UnitDelay: '<Root>/Unit Delay1'

  tmp_0[12] = rtU.reference[0];
  tmp_0[13] = rtU.reference[1];
  tmp_0[14] = rtU.reference[2];
  tmp_1[12] = tmp_8;
  tmp_1[13] = tmp_9;
  tmp_1[14] = tmp_a;
  std::memcpy(&tmp_1[0], &tmp[0], 12U * sizeof(real_T));
  std::memcpy(&tmp_2[0], &rtDW.UnitDelay1_DSTATE[0], 12U * sizeof(real_T));
  tmp_2[12] = 0.0;
  tmp_2[13] = 0.0;
  tmp_2[14] = 0.0;
  for (i = 0; i < 15; i++) {
    // DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
    //   Sum: '<Root>/state_derivative_sum'

    rtb_passthrough_gain_0 = ((tmp_0[i] + tmp_1[i]) + tmp_2[i]) * 0.025;
    state_with_error_tmp[i] = rtb_passthrough_gain_0;

    // DiscreteIntegrator: '<Root>/Discrete Integrator'
    state_with_error[i] = rtDW.DiscreteIntegrator_DSTATE[i] +
      rtb_passthrough_gain_0;
  }

  // Outport: '<Root>/Out17'
  rtY.altitude_error = state_with_error[12];

  // Outport: '<Root>/Out18'
  rtY.crossrange_error = state_with_error[13];

  // Outport: '<Root>/Out19'
  rtY.downrange_error = state_with_error[14];

  // Gain: '<Root>/lqi_gain'
  tmp_8 = 0.0;
  tmp_9 = 0.0;
  tmp_a = 0.0;
  tmp_6 = 0.0;
  for (i = 0; i < 3; i++) {
    rtb_passthrough_gain_0 = state_with_error[i + 12];
    i_0 = i << 2;
    tmp_8 += rtConstP.lqi_gain_Gain[i_0] * rtb_passthrough_gain_0;
    tmp_9 += rtConstP.lqi_gain_Gain[i_0 + 1] * rtb_passthrough_gain_0;
    tmp_a += rtConstP.lqi_gain_Gain[i_0 + 2] * rtb_passthrough_gain_0;
    tmp_6 += rtConstP.lqi_gain_Gain[i_0 + 3] * rtb_passthrough_gain_0;
  }

  // End of Gain: '<Root>/lqi_gain'

  // Gain: '<Root>/lqr_gain'
  tmp_3 = 0.0;
  tmp_4 = 0.0;
  tmp_5 = 0.0;
  tmp_7 = 0.0;
  for (i = 0; i < 12; i++) {
    rtb_passthrough_gain_0 = state_with_error[i];
    i_0 = i << 2;
    tmp_3 += rtConstP.lqr_gain_Gain[i_0] * rtb_passthrough_gain_0;
    tmp_4 += rtConstP.lqr_gain_Gain[i_0 + 1] * rtb_passthrough_gain_0;
    tmp_5 += rtConstP.lqr_gain_Gain[i_0 + 2] * rtb_passthrough_gain_0;
    tmp_7 += rtConstP.lqr_gain_Gain[i_0 + 3] * rtb_passthrough_gain_0;
  }

  // End of Gain: '<Root>/lqr_gain'

  // Saturate: '<S1>/saturator' incorporates:
  //   Constant: '<Root>/ff_control'
  //   SignalConversion generated from: '<Root>/input_gain'
  //   Sum: '<Root>/control_sum'

  tmp_8 = (tmp_8 + tmp_3) + 1.7400000000000002;
  if (tmp_8 > 3.5) {
    tmp_8 = 3.5;
    rtb_control[0] = 3.5;
  } else if (tmp_8 < 0.0) {
    tmp_8 = 0.0;
    rtb_control[0] = 0.0;
  } else {
    rtb_control[0] = tmp_8;
  }

  tmp_9 += tmp_4;
  if (tmp_9 > 1.0) {
    tmp_9 = 1.0;
    rtb_control[1] = 1.0;
  } else if (tmp_9 < -1.0) {
    tmp_9 = -1.0;
    rtb_control[1] = -1.0;
  } else {
    rtb_control[1] = tmp_9;
  }

  tmp_a += tmp_5;
  if (tmp_a > 0.14279966607226333) {
    tmp_a = 0.14279966607226333;
    rtb_control[2] = 0.14279966607226333;
  } else if (tmp_a < -0.14279966607226333) {
    tmp_a = -0.14279966607226333;
    rtb_control[2] = -0.14279966607226333;
  } else {
    rtb_control[2] = tmp_a;
  }

  tmp_6 += tmp_7;
  if (tmp_6 > 0.14279966607226333) {
    tmp_6 = 0.14279966607226333;
    rtb_control[3] = 0.14279966607226333;
  } else if (tmp_6 < -0.14279966607226333) {
    tmp_6 = -0.14279966607226333;
    rtb_control[3] = -0.14279966607226333;
  } else {
    rtb_control[3] = tmp_6;
  }

  // RateLimiter: '<S1>/Rate Limiter3'
  rtDW.PrevY = rtb_control[0];

  // RateLimiter: '<S1>/Rate Limiter2'
  rtDW.PrevY_k = rtb_control[1];

  // RateLimiter: '<S1>/Rate Limiter1'
  rtDW.PrevY_b = rtb_control[2];

  // RateLimiter: '<S1>/Rate Limiter'
  rtDW.PrevY_l = rtb_control[3];

  // Gain: '<Root>/passthrough_gain'
  std::memset(&tmp[0], 0, 12U * sizeof(real_T));
  for (i = 0; i < 4; i++) {
    rtb_passthrough_gain_0 = rtb_control[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      tmp[i_0] += 0.0 * rtb_passthrough_gain_0;
    }
  }

  // End of Gain: '<Root>/passthrough_gain'

  // Sum: '<Root>/Sum2' incorporates:
  //   Gain: '<Root>/output_gain'

  for (i = 0; i < 12; i++) {
    rtb_passthrough_gain_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtb_passthrough_gain_0 += rtConstP.output_gain_Gain[12 * i_0 + i] *
        state_with_error[i_0];
    }

    rtb_passthrough_gain[i] = rtb_passthrough_gain_0 + tmp[i];
  }

  // End of Sum: '<Root>/Sum2'

  // Outport: '<Root>/Out1'
  rtY.altitude = rtb_passthrough_gain[0];

  // Outport: '<Root>/Out2'
  rtY.crossrange = rtb_passthrough_gain[1];

  // Outport: '<Root>/Out3'
  rtY.downrange = rtb_passthrough_gain[2];

  // Outport: '<Root>/Out4'
  rtY.lateral_vel = rtb_passthrough_gain[3];

  // Outport: '<Root>/Out5'
  rtY.longitudinal_vel = rtb_passthrough_gain[4];

  // Outport: '<Root>/Out6'
  rtY.directional_vel = rtb_passthrough_gain[5];

  // Outport: '<Root>/control_commands_out' incorporates:
  //   Saturate: '<S1>/saturator'

  rtY.control[0] = tmp_8;
  rtY.control[1] = tmp_9;
  rtY.control[2] = tmp_a;
  rtY.control[3] = tmp_6;

  // Outport: '<Root>/Out7' incorporates:
  //   Gain: '<Root>/Gain12'

  rtY.roll = 57.295779513082323 * rtb_passthrough_gain[6];

  // Outport: '<Root>/Out8' incorporates:
  //   Gain: '<Root>/Gain11'

  rtY.pitch = 57.295779513082323 * rtb_passthrough_gain[7];

  // Outport: '<Root>/Out9' incorporates:
  //   Gain: '<Root>/Gain10'

  rtY.yaw = 57.295779513082323 * rtb_passthrough_gain[8];

  // Outport: '<Root>/Out10' incorporates:
  //   Gain: '<Root>/Gain9'

  rtY.lateral_rate = 57.295779513082323 * rtb_passthrough_gain[9];

  // Outport: '<Root>/Out11' incorporates:
  //   Gain: '<Root>/Gain8'

  rtY.longitudinal_rate = 57.295779513082323 * rtb_passthrough_gain[10];

  // Outport: '<Root>/Out12' incorporates:
  //   Gain: '<Root>/Gain7'

  rtY.directional_rate = 57.295779513082323 * rtb_passthrough_gain[11];

  // Outport: '<Root>/Out16' incorporates:
  //   Gain: '<Root>/Gain6'
  //   Saturate: '<S1>/saturator'

  rtY.lower_gimbal_angle = 57.295779513082323 * tmp_6;

  // Outport: '<Root>/Out15' incorporates:
  //   Gain: '<Root>/Gain5'
  //   Saturate: '<S1>/saturator'

  rtY.upper_gimbal_angle = 57.295779513082323 * tmp_a;

  // Outport: '<Root>/Out14' incorporates:
  //   Saturate: '<S1>/saturator'

  rtY.reaction_torque = tmp_9;

  // Outport: '<Root>/Out13' incorporates:
  //   Saturate: '<S1>/saturator'

  rtY.thrust = tmp_8;
  for (i = 0; i < 12; i++) {
    // Update for UnitDelay: '<Root>/Unit Delay' incorporates:
    //   Inport: '<Root>/imu_state_in'

    rtDW.UnitDelay_DSTATE[i] = rtU.imu_state_in[i];

    // Gain: '<Root>/input_gain' incorporates:
    //   UnitDelay: '<Root>/Unit Delay1'

    rtDW.UnitDelay1_DSTATE[i] = 0.0;
  }

  // Gain: '<Root>/input_gain' incorporates:
  //   UnitDelay: '<Root>/Unit Delay1'

  for (i = 0; i < 4; i++) {
    rtb_passthrough_gain_0 = rtb_control[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtDW.UnitDelay1_DSTATE[i_0] += rtConstP.input_gain_Gain[12 * i + i_0] *
        rtb_passthrough_gain_0;
    }
  }

  // Update for DiscreteIntegrator: '<Root>/Discrete Integrator'
  for (i = 0; i < 15; i++) {
    rtDW.DiscreteIntegrator_DSTATE[i] = state_with_error[i] +
      state_with_error_tmp[i];
  }
}

// Model initialize function
void skipperLQR::initialize()
{
  // InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
  //   Constant: '<Root>/initial_state'

  std::memcpy(&rtDW.DiscreteIntegrator_DSTATE[0], &rtConstB.initial_state[0],
              12U * sizeof(real_T));
  rtDW.DiscreteIntegrator_DSTATE[12] = rtConstB.initial_error[0];
  rtDW.DiscreteIntegrator_DSTATE[13] = rtConstB.initial_error[1];
  rtDW.DiscreteIntegrator_DSTATE[14] = rtConstB.initial_error[2];

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter2'
  rtDW.PrevY_k = 1.7;

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter1'
  rtDW.PrevY_b = 1.7;

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter'
  rtDW.PrevY_l = 1.7;
}

// Constructor
skipperLQR::skipperLQR():
  rtU(),
  rtY(),
  rtDW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
skipperLQR::~skipperLQR() = default;
extern "C"
{
  // Test if value is infinite
  static boolean_T rtIsInf(real_T value)
  {
    return std::isinf(value);
  }

  // Test if single-precision value is infinite
  static boolean_T rtIsInfF(real32_T value)
  {
    return std::isinf(value);
  }

  // Test if value is not a number
  static boolean_T rtIsNaN(real_T value)
  {
    return std::isnan(value);
  }

  // Test if single-precision value is not a number
  static boolean_T rtIsNaNF(real32_T value)
  {
    return std::isnan(value);
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
