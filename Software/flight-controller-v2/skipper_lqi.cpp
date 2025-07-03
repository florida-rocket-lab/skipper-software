/*
 * skipper_lqi.cpp
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi".
 *
 * Model version              : 1.118
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C++ source code generated on : Thu Jul  3 17:15:18 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#include "skipper_lqi.h"
#include <cstring>
#include "rtwtypes.h"

/* Model step function */
void skipper_lqi::step()
{
  real_T state_with_error[15];
  real_T state_with_error_tmp[15];
  real_T tmp_1[15];
  real_T tmp_2[15];
  real_T tmp_3[15];
  real_T rtb_passthrough_gain[12];
  real_T tmp[12];
  real_T rtb_saturator[4];
  real_T tmp_0[3];
  real_T state_with_error_tmp_0;
  real_T u1;
  real_T u2;
  int32_T i;
  int32_T i_0;

  /* Sum: '<Root>/feedback_sum' incorporates:
   *  UnitDelay: '<Root>/Unit Delay'
   */
  std::memcpy(&rtb_passthrough_gain[0], &skipper_lqi_DW.UnitDelay_DSTATE[0], 12U
              * sizeof(real_T));

  /* Gain: '<Root>/state_gain' */
  for (i_0 = 0; i_0 < 12; i_0++) {
    state_with_error_tmp_0 = 0.0;
    for (i = 0; i < 12; i++) {
      state_with_error_tmp_0 += skipper_lqi_ConstP.state_gain_Gain[12 * i + i_0]
        * rtb_passthrough_gain[i];
    }

    tmp[i_0] = state_with_error_tmp_0;
  }

  /* End of Gain: '<Root>/state_gain' */

  /* Gain: '<Root>/extraction_gain' */
  for (i_0 = 0; i_0 < 3; i_0++) {
    state_with_error_tmp_0 = 0.0;
    for (i = 0; i < 12; i++) {
      state_with_error_tmp_0 += skipper_lqi_ConstP.extraction_gain_Gain[3 * i +
        i_0] * rtb_passthrough_gain[i];
    }

    tmp_0[i_0] = state_with_error_tmp_0;
  }

  /* End of Gain: '<Root>/extraction_gain' */

  /* Sum: '<Root>/state_derivative_sum' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Constant: '<Root>/gravity_const'
   *  Constant: '<Root>/reference_const'
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  tmp_1[12] = 10.0;
  tmp_1[13] = 5.0;
  tmp_1[14] = 5.0;
  tmp_2[12] = tmp_0[0];
  tmp_2[13] = tmp_0[1];
  tmp_2[14] = tmp_0[2];
  std::memcpy(&tmp_1[0], &skipper_lqi_ConstP.gravity_const_Value[0], 12U *
              sizeof(real_T));
  std::memcpy(&tmp_2[0], &tmp[0], 12U * sizeof(real_T));
  std::memcpy(&tmp_3[0], &skipper_lqi_DW.UnitDelay1_DSTATE[0], 12U * sizeof
              (real_T));
  tmp_3[12] = 0.0;
  tmp_3[13] = 0.0;
  tmp_3[14] = 0.0;
  for (i = 0; i < 15; i++) {
    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    state_with_error_tmp_0 = ((tmp_1[i] + tmp_2[i]) + tmp_3[i]) * 0.025;
    state_with_error_tmp[i] = state_with_error_tmp_0;

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' */
    state_with_error[i] = skipper_lqi_DW.DiscreteIntegrator_DSTATE[i] +
      state_with_error_tmp_0;
  }

  for (i_0 = 0; i_0 < 4; i_0++) {
    /* Gain: '<Root>/lqr_gain' */
    state_with_error_tmp_0 = 0.0;
    for (i = 0; i < 12; i++) {
      state_with_error_tmp_0 += skipper_lqi_ConstP.lqr_gain_Gain[(i << 2) + i_0]
        * state_with_error[i];
    }

    /* Saturate: '<S1>/saturator' incorporates:
     *  Constant: '<Root>/ff_control'
     *  Gain: '<Root>/lqi_gain'
     *  Gain: '<Root>/lqr_gain'
     *  Sum: '<Root>/control_sum'
     */
    state_with_error_tmp_0 = (((skipper_lqi_ConstP.lqi_gain_Gain[i_0 + 4] *
      state_with_error[13] + skipper_lqi_ConstP.lqi_gain_Gain[i_0] *
      state_with_error[12]) + skipper_lqi_ConstP.lqi_gain_Gain[i_0 + 8] *
      state_with_error[14]) + state_with_error_tmp_0) +
      skipper_lqi_ConstP.ff_control_Value[i_0];
    u1 = skipper_lqi_ConstP.saturator_LowerSat[i_0];
    u2 = skipper_lqi_ConstP.saturator_UpperSat[i_0];
    if (state_with_error_tmp_0 > u2) {
      rtb_saturator[i_0] = u2;
    } else if (state_with_error_tmp_0 < u1) {
      rtb_saturator[i_0] = u1;
    } else {
      rtb_saturator[i_0] = state_with_error_tmp_0;
    }

    /* End of Saturate: '<S1>/saturator' */
  }

  /* Outport: '<Root>/Out13' incorporates:
   *  RateLimiter: '<S1>/Rate Limiter3'
   */
  skipper_lqi_Y.thrust = rtb_saturator[0];

  /* RateLimiter: '<S1>/Rate Limiter3' */
  skipper_lqi_DW.PrevY = rtb_saturator[0];

  /* Outport: '<Root>/Out14' incorporates:
   *  RateLimiter: '<S1>/Rate Limiter2'
   */
  skipper_lqi_Y.reaction_torque = rtb_saturator[1];

  /* RateLimiter: '<S1>/Rate Limiter2' */
  skipper_lqi_DW.PrevY_k = rtb_saturator[1];

  /* RateLimiter: '<S1>/Rate Limiter1' */
  skipper_lqi_DW.PrevY_b = rtb_saturator[2];

  /* RateLimiter: '<S1>/Rate Limiter' */
  skipper_lqi_DW.PrevY_l = rtb_saturator[3];

  /* Outport: '<Root>/Out15' incorporates:
   *  Gain: '<Root>/Gain5'
   *  RateLimiter: '<S1>/Rate Limiter1'
   */
  skipper_lqi_Y.upper_gimbal_angle = 57.295779513082323 * rtb_saturator[2];

  /* Outport: '<Root>/Out16' incorporates:
   *  Gain: '<Root>/Gain6'
   *  RateLimiter: '<S1>/Rate Limiter'
   */
  skipper_lqi_Y.lower_gimbal_angle = 57.295779513082323 * rtb_saturator[3];

  /* Outport: '<Root>/Out17' */
  skipper_lqi_Y.altitude_error = state_with_error[12];

  /* Outport: '<Root>/Out18' */
  skipper_lqi_Y.crossrange_error = state_with_error[13];

  /* Outport: '<Root>/Out19' */
  skipper_lqi_Y.downrange_error = state_with_error[14];
  for (i = 0; i < 12; i++) {
    /* Gain: '<Root>/output_gain' */
    state_with_error_tmp_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      state_with_error_tmp_0 += skipper_lqi_ConstP.output_gain_Gain[12 * i_0 + i]
        * state_with_error[i_0];
    }

    /* Update for UnitDelay: '<Root>/Unit Delay' */
    skipper_lqi_DW.UnitDelay_DSTATE[i] = state_with_error[i];

    /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
     *  Gain: '<Root>/input_gain'
     *  Gain: '<Root>/passthrough_gain'
     */
    skipper_lqi_DW.UnitDelay1_DSTATE[i] = ((skipper_lqi_ConstP.input_gain_Gain[i
      + 12] * rtb_saturator[1] + skipper_lqi_ConstP.input_gain_Gain[i] *
      rtb_saturator[0]) + skipper_lqi_ConstP.input_gain_Gain[i + 24] *
      rtb_saturator[2]) + skipper_lqi_ConstP.input_gain_Gain[i + 36] *
      rtb_saturator[3];

    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/output_gain'
     *  Gain: '<Root>/passthrough_gain'
     */
    rtb_passthrough_gain[i] = (((0.0 * rtb_saturator[0] + 0.0 * rtb_saturator[1])
      + 0.0 * rtb_saturator[2]) + 0.0 * rtb_saturator[3]) +
      state_with_error_tmp_0;
  }

  /* Outport: '<Root>/Out1' */
  skipper_lqi_Y.altitude = rtb_passthrough_gain[0];

  /* Outport: '<Root>/Out2' */
  skipper_lqi_Y.crossrange = rtb_passthrough_gain[1];

  /* Outport: '<Root>/Out3' */
  skipper_lqi_Y.downrange = rtb_passthrough_gain[2];

  /* Outport: '<Root>/Out4' */
  skipper_lqi_Y.lateral_vel = rtb_passthrough_gain[3];

  /* Outport: '<Root>/Out5' */
  skipper_lqi_Y.longitudinal_vel = rtb_passthrough_gain[4];

  /* Outport: '<Root>/Out6' */
  skipper_lqi_Y.directional_vel = rtb_passthrough_gain[5];

  /* Outport: '<Root>/Out7' incorporates:
   *  Gain: '<Root>/Gain12'
   */
  skipper_lqi_Y.roll = 57.295779513082323 * rtb_passthrough_gain[6];

  /* Outport: '<Root>/Out8' incorporates:
   *  Gain: '<Root>/Gain11'
   */
  skipper_lqi_Y.pitch = 57.295779513082323 * rtb_passthrough_gain[7];

  /* Outport: '<Root>/Out9' incorporates:
   *  Gain: '<Root>/Gain10'
   */
  skipper_lqi_Y.yaw = 57.295779513082323 * rtb_passthrough_gain[8];

  /* Outport: '<Root>/Out10' incorporates:
   *  Gain: '<Root>/Gain9'
   */
  skipper_lqi_Y.lateral_rate = 57.295779513082323 * rtb_passthrough_gain[9];

  /* Outport: '<Root>/Out11' incorporates:
   *  Gain: '<Root>/Gain8'
   */
  skipper_lqi_Y.longitudinal_rate = 57.295779513082323 * rtb_passthrough_gain[10];

  /* Outport: '<Root>/Out12' incorporates:
   *  Gain: '<Root>/Gain7'
   */
  skipper_lqi_Y.directional_rate = 57.295779513082323 * rtb_passthrough_gain[11];

  /* Update for DiscreteIntegrator: '<Root>/Discrete Integrator' */
  for (i = 0; i < 15; i++) {
    skipper_lqi_DW.DiscreteIntegrator_DSTATE[i] = state_with_error[i] +
      state_with_error_tmp[i];
  }
}

/* Model initialize function */
void skipper_lqi::initialize()
{
  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
   *  Constant: '<Root>/initial_state'
   */
  std::memcpy(&skipper_lqi_DW.DiscreteIntegrator_DSTATE[0],
              &skipper_lqi_ConstB.initial_state[0], 12U * sizeof(real_T));
  skipper_lqi_DW.DiscreteIntegrator_DSTATE[12] =
    skipper_lqi_ConstB.initial_error[0];
  skipper_lqi_DW.DiscreteIntegrator_DSTATE[13] =
    skipper_lqi_ConstB.initial_error[1];
  skipper_lqi_DW.DiscreteIntegrator_DSTATE[14] =
    skipper_lqi_ConstB.initial_error[2];

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter2' */
  skipper_lqi_DW.PrevY_k = 1.7;

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter1' */
  skipper_lqi_DW.PrevY_b = 1.7;

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter' */
  skipper_lqi_DW.PrevY_l = 1.7;
}

/* Model terminate function */
void skipper_lqi::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
skipper_lqi::skipper_lqi() :
  skipper_lqi_Y(),
  skipper_lqi_DW(),
  skipper_lqi_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
/* Currently there is no destructor body generated.*/
skipper_lqi::~skipper_lqi() = default;

/* Real-Time Model get method */
RT_MODEL_skipper_lqi_T * skipper_lqi::getRTM()
{
  return (&skipper_lqi_M);
}
