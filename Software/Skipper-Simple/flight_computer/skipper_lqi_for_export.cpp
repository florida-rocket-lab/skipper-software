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
// C/C++ source code generated on : Thu Sep 25 15:38:50 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "skipper_lqi_for_export.h"
#include <cstring>
#include "rtwtypes.h"

// Model step function
void skipperLQR::step()
{
  real_T state_with_error_tmp[15];
  real_T tmp_0[15];
  real_T tmp_1[15];
  real_T tmp_2[15];
  real_T rtb_output[12];
  real_T tmp[12];
  real_T UnitDelay_DSTATE;
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

  // Gain: '<Root>/state_gain'
  std::memset(&tmp[0], 0, 12U * sizeof(real_T));

  // Gain: '<Root>/extraction_gain'
  tmp_8 = 0.0;
  tmp_9 = 0.0;
  tmp_a = 0.0;
  for (i = 0; i < 12; i++) {
    // Gain: '<Root>/state_gain' incorporates:
    //   Gain: '<Root>/passthrough_gain'

    UnitDelay_DSTATE = skipper_lqi_for_export_DW.UnitDelay_DSTATE[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      tmp[i_0] += skipper_lqi_for_export_ConstP.state_gain_Gain[12 * i + i_0] *
        UnitDelay_DSTATE;
    }

    // Gain: '<Root>/extraction_gain' incorporates:
    //   Gain: '<Root>/passthrough_gain'

    tmp_8 += skipper_lqi_for_export_ConstP.extraction_gain_Gain[3 * i] *
      UnitDelay_DSTATE;
    tmp_9 += skipper_lqi_for_export_ConstP.extraction_gain_Gain[3 * i + 1] *
      UnitDelay_DSTATE;
    tmp_a += skipper_lqi_for_export_ConstP.extraction_gain_Gain[3 * i + 2] *
      UnitDelay_DSTATE;

    // Sum: '<Root>/state_derivative_sum' incorporates:
    //   Constant: '<Root>/gravity_const'

    tmp_0[i] = skipper_lqi_for_export_ConstP.gravity_const_Value[i];
  }

  // Sum: '<Root>/state_derivative_sum' incorporates:
  //   Constant: '<Root>/dummy_zeros'
  //   Inport: '<Root>/refeference_in'
  //   UnitDelay: '<Root>/Unit Delay1'

  tmp_0[12] = skipper_lqi_for_export_U.reference[0];
  tmp_0[13] = skipper_lqi_for_export_U.reference[1];
  tmp_0[14] = skipper_lqi_for_export_U.reference[2];
  tmp_1[12] = tmp_8;
  tmp_1[13] = tmp_9;
  tmp_1[14] = tmp_a;
  std::memcpy(&tmp_1[0], &tmp[0], 12U * sizeof(real_T));
  std::memcpy(&tmp_2[0], &skipper_lqi_for_export_DW.UnitDelay1_DSTATE[0], 12U *
              sizeof(real_T));
  tmp_2[12] = 0.0;
  tmp_2[13] = 0.0;
  tmp_2[14] = 0.0;
  for (i = 0; i < 15; i++) {
    // DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
    //   Sum: '<Root>/state_derivative_sum'

    UnitDelay_DSTATE = ((tmp_0[i] + tmp_1[i]) + tmp_2[i]) * 0.025;
    state_with_error_tmp[i] = UnitDelay_DSTATE;

    // DiscreteIntegrator: '<Root>/Discrete Integrator'
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i] += UnitDelay_DSTATE;
  }

  // Outport: '<Root>/Out17'
  skipper_lqi_for_export_Y.altitude_error =
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[12];

  // Outport: '<Root>/Out18'
  skipper_lqi_for_export_Y.crossrange_error =
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[13];

  // Outport: '<Root>/Out19'
  skipper_lqi_for_export_Y.downrange_error =
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[14];

  // Gain: '<Root>/lqi_gain'
  tmp_8 = 0.0;
  tmp_9 = 0.0;
  tmp_a = 0.0;
  tmp_6 = 0.0;
  for (i = 0; i < 3; i++) {
    UnitDelay_DSTATE = skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i +
      12];
    i_0 = i << 2;
    tmp_8 += skipper_lqi_for_export_ConstP.lqi_gain_Gain[i_0] * UnitDelay_DSTATE;
    tmp_9 += skipper_lqi_for_export_ConstP.lqi_gain_Gain[i_0 + 1] *
      UnitDelay_DSTATE;
    tmp_a += skipper_lqi_for_export_ConstP.lqi_gain_Gain[i_0 + 2] *
      UnitDelay_DSTATE;
    tmp_6 += skipper_lqi_for_export_ConstP.lqi_gain_Gain[i_0 + 3] *
      UnitDelay_DSTATE;
  }

  // End of Gain: '<Root>/lqi_gain'

  // Gain: '<Root>/lqr_gain'
  tmp_3 = 0.0;
  tmp_4 = 0.0;
  tmp_5 = 0.0;
  tmp_7 = 0.0;
  for (i = 0; i < 12; i++) {
    UnitDelay_DSTATE = skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i];
    i_0 = i << 2;
    tmp_3 += skipper_lqi_for_export_ConstP.lqr_gain_Gain[i_0] * UnitDelay_DSTATE;
    tmp_4 += skipper_lqi_for_export_ConstP.lqr_gain_Gain[i_0 + 1] *
      UnitDelay_DSTATE;
    tmp_5 += skipper_lqi_for_export_ConstP.lqr_gain_Gain[i_0 + 2] *
      UnitDelay_DSTATE;
    tmp_7 += skipper_lqi_for_export_ConstP.lqr_gain_Gain[i_0 + 3] *
      UnitDelay_DSTATE;
  }

  // End of Gain: '<Root>/lqr_gain'

  // Saturate: '<S1>/saturator' incorporates:
  //   Constant: '<Root>/ff_control'
  //   SignalConversion generated from: '<Root>/input_gain'
  //   Sum: '<Root>/control_sum'

  UnitDelay_DSTATE = (tmp_8 + tmp_3) + 1.7400000000000002;
  if (UnitDelay_DSTATE > 3.5) {
    skipper_lqi_for_export_Y.control[0] = 2.65;
  } else if (UnitDelay_DSTATE < 0.0) {
    skipper_lqi_for_export_Y.control[0] = 0.0;
  } else {
    skipper_lqi_for_export_Y.control[0] = UnitDelay_DSTATE;
  }

  UnitDelay_DSTATE = tmp_9 + tmp_4;
  if (UnitDelay_DSTATE > 1.0) {
    skipper_lqi_for_export_Y.control[1] = 1.0;
  } else if (UnitDelay_DSTATE < -1.0) {
    skipper_lqi_for_export_Y.control[1] = -1.0;
  } else {
    skipper_lqi_for_export_Y.control[1] = UnitDelay_DSTATE;
  }

  UnitDelay_DSTATE = tmp_a + tmp_5;
  if (UnitDelay_DSTATE > 0.14279966607226333) {
    skipper_lqi_for_export_Y.control[2] = 0.14279966607226333;
  } else if (UnitDelay_DSTATE < -0.14279966607226333) {
    skipper_lqi_for_export_Y.control[2] = -0.14279966607226333;
  } else {
    skipper_lqi_for_export_Y.control[2] = UnitDelay_DSTATE;
  }

  UnitDelay_DSTATE = tmp_6 + tmp_7;
  if (UnitDelay_DSTATE > 0.14279966607226333) {
    skipper_lqi_for_export_Y.control[3] = 0.14279966607226333;
  } else if (UnitDelay_DSTATE < -0.14279966607226333) {
    skipper_lqi_for_export_Y.control[3] = -0.14279966607226333;
  } else {
    skipper_lqi_for_export_Y.control[3] = UnitDelay_DSTATE;
  }

  // End of Saturate: '<S1>/saturator'

  // Outport: '<Root>/Out13' incorporates:
  //   RateLimiter: '<S1>/Rate Limiter3'

  skipper_lqi_for_export_Y.thrust = skipper_lqi_for_export_Y.control[0];

  // RateLimiter: '<S1>/Rate Limiter3'
  skipper_lqi_for_export_DW.PrevY = skipper_lqi_for_export_Y.control[0];

  // Outport: '<Root>/Out14' incorporates:
  //   RateLimiter: '<S1>/Rate Limiter2'

  skipper_lqi_for_export_Y.reaction_torque = skipper_lqi_for_export_Y.control[1];

  // RateLimiter: '<S1>/Rate Limiter2'
  skipper_lqi_for_export_DW.PrevY_k = skipper_lqi_for_export_Y.control[1];

  // RateLimiter: '<S1>/Rate Limiter1'
  skipper_lqi_for_export_DW.PrevY_b = skipper_lqi_for_export_Y.control[2];

  // RateLimiter: '<S1>/Rate Limiter'
  skipper_lqi_for_export_DW.PrevY_l = skipper_lqi_for_export_Y.control[3];

  // Gain: '<Root>/passthrough_gain'
  std::memset(&tmp[0], 0, 12U * sizeof(real_T));
  for (i = 0; i < 4; i++) {
    UnitDelay_DSTATE = skipper_lqi_for_export_Y.control[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      tmp[i_0] += 0.0 * UnitDelay_DSTATE;
    }
  }

  // End of Gain: '<Root>/passthrough_gain'

  // Sum: '<Root>/Sum2' incorporates:
  //   Gain: '<Root>/output_gain'

  for (i = 0; i < 12; i++) {
    UnitDelay_DSTATE = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      UnitDelay_DSTATE += skipper_lqi_for_export_ConstP.output_gain_Gain[12 *
        i_0 + i] * skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i_0];
    }

    rtb_output[i] = UnitDelay_DSTATE + tmp[i];
  }

  // End of Sum: '<Root>/Sum2'

  // Outport: '<Root>/Out1'
  skipper_lqi_for_export_Y.altitude = rtb_output[0];

  // Outport: '<Root>/Out2'
  skipper_lqi_for_export_Y.crossrange = rtb_output[1];

  // Outport: '<Root>/Out3'
  skipper_lqi_for_export_Y.downrange = rtb_output[2];

  // Outport: '<Root>/Out4'
  skipper_lqi_for_export_Y.lateral_vel = rtb_output[3];

  // Outport: '<Root>/Out5'
  skipper_lqi_for_export_Y.longitudinal_vel = rtb_output[4];

  // Outport: '<Root>/Out6'
  skipper_lqi_for_export_Y.directional_vel = rtb_output[5];

  // Gain: '<Root>/input_gain' incorporates:
  //   UnitDelay: '<Root>/Unit Delay1'

  std::memset(&skipper_lqi_for_export_DW.UnitDelay1_DSTATE[0], 0, 12U * sizeof
              (real_T));
  for (i = 0; i < 4; i++) {
    UnitDelay_DSTATE = skipper_lqi_for_export_Y.control[i];
    for (i_0 = 0; i_0 < 12; i_0++) {
      skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i_0] +=
        skipper_lqi_for_export_ConstP.input_gain_Gain[12 * i + i_0] *
        UnitDelay_DSTATE;
    }
  }

  // End of Gain: '<Root>/input_gain'

  // Outport: '<Root>/Out7' incorporates:
  //   Gain: '<Root>/Gain12'

  skipper_lqi_for_export_Y.roll = 57.295779513082323 * rtb_output[6];

  // Outport: '<Root>/Out8' incorporates:
  //   Gain: '<Root>/Gain11'

  skipper_lqi_for_export_Y.pitch = 57.295779513082323 * rtb_output[7];

  // Outport: '<Root>/Out9' incorporates:
  //   Gain: '<Root>/Gain10'

  skipper_lqi_for_export_Y.yaw = 57.295779513082323 * rtb_output[8];

  // Outport: '<Root>/Out10' incorporates:
  //   Gain: '<Root>/Gain9'

  skipper_lqi_for_export_Y.lateral_rate = 57.295779513082323 * rtb_output[9];

  // Outport: '<Root>/Out11' incorporates:
  //   Gain: '<Root>/Gain8'

  skipper_lqi_for_export_Y.longitudinal_rate = 57.295779513082323 * rtb_output
    [10];

  // Outport: '<Root>/Out12' incorporates:
  //   Gain: '<Root>/Gain7'

  skipper_lqi_for_export_Y.directional_rate = 57.295779513082323 * rtb_output[11];

  // Outport: '<Root>/Out16' incorporates:
  //   Gain: '<Root>/Gain6'
  //   RateLimiter: '<S1>/Rate Limiter'

  skipper_lqi_for_export_Y.lower_gimbal_angle = 57.295779513082323 *
    skipper_lqi_for_export_Y.control[3];

  // Outport: '<Root>/Out15' incorporates:
  //   Gain: '<Root>/Gain5'
  //   RateLimiter: '<S1>/Rate Limiter1'

  skipper_lqi_for_export_Y.upper_gimbal_angle = 57.295779513082323 *
    skipper_lqi_for_export_Y.control[2];

  // Update for UnitDelay: '<Root>/Unit Delay' incorporates:
  //   Gain: '<Root>/passthrough_gain'
  //   Inport: '<Root>/imu_state_in'

  std::memcpy(&skipper_lqi_for_export_DW.UnitDelay_DSTATE[0],
              &skipper_lqi_for_export_U.imu_state_in[0], 12U * sizeof(real_T));

  // Update for DiscreteIntegrator: '<Root>/Discrete Integrator'
  for (i = 0; i < 15; i++) {
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i] +=
      state_with_error_tmp[i];
  }
}

// Model initialize function
void skipperLQR::initialize()
{
  // InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
  //   Constant: '<Root>/initial_state'

  std::memcpy(&skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[0],
              &skipper_lqi_for_export_ConstB.initial_state[0], 12U * sizeof
              (real_T));
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[12] =
    skipper_lqi_for_export_ConstB.initial_error[0];
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[13] =
    skipper_lqi_for_export_ConstB.initial_error[1];
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[14] =
    skipper_lqi_for_export_ConstB.initial_error[2];

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter2'
  skipper_lqi_for_export_DW.PrevY_k = 1.7;

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter1'
  skipper_lqi_for_export_DW.PrevY_b = 1.7;

  // InitializeConditions for RateLimiter: '<S1>/Rate Limiter'
  skipper_lqi_for_export_DW.PrevY_l = 1.7;
}

// Model terminate function
void skipperLQR::terminate()
{
  // (no terminate code required)
}

const char_T* skipperLQR::RT_MODEL_skipper_lqi_for_expo_T::getErrorStatus()
  const
{
  return (errorStatus);
}

void skipperLQR::RT_MODEL_skipper_lqi_for_expo_T::setErrorStatus(const char_T*
  const volatile aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

// Constructor
skipperLQR::skipperLQR() :
  skipper_lqi_for_export_U(),
  skipper_lqi_for_export_Y(),
  skipper_lqi_for_export_DW(),
  skipper_lqi_for_export_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
skipperLQR::~skipperLQR() = default;

// Real-Time Model get method
skipperLQR::RT_MODEL_skipper_lqi_for_expo_T * skipperLQR::getRTM()
{
  return (&skipper_lqi_for_export_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
