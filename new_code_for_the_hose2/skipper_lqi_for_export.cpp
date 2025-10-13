/*
 * skipper_lqi_for_export.cpp
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi_for_export".
 *
 * Model version              : 3.3
 * Simulink Coder version : 24.1 (R2024a) 19-Nov-2023
 * C++ source code generated on : Mon Oct 13 16:19:07 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#include "skipper_lqi_for_export.h"
#include "rtwtypes.h"
#include <cstring>
#include <emmintrin.h>
#include "skipper_lqi_for_export_private.h"

const real_T skipper_lqi_for_export_period{ 0.05 };

/* Model step function */
void skipper_lqi_for_export::step()
{
  __m128d tmp_3;
  __m128d tmp_4;
  __m128d tmp_5;
  real_T tmp_1[48];
  real_T tmp[36];
  real_T tmp_0[12];
  real_T tmp_6[2];
  const real_T *tmp_7;
  real_T control;
  real_T state_gain;
  real_T state_with_error;
  real_T state_with_error_0;
  real_T tmp_2;
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  for (i = 0; i <= 10; i += 2) {
    /* UnitDelay: '<Root>/Unit Delay' */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_DW.UnitDelay_DSTATE[i]);
    _mm_storeu_pd(&skipper_lqi_for_export_B.UnitDelay[i], tmp_4);

    /* Sum: '<Root>/feedback_sum' incorporates:
     *  Constant: '<Root>/perturbation'
     *  UnitDelay: '<Root>/Unit Delay'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.perturbation_Value[i]);
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Sum: '<Root>/feedback_sum' incorporates:
     *  UnitDelay: '<Root>/Unit Delay'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_B.state_with_perturbation[i], tmp_4);
  }

  for (i_0 = 0; i_0 < 12; i_0++) {
    /* Gain: '<Root>/state_gain' */
    state_gain = 0.0;
    for (i = 0; i < 12; i++) {
      state_gain += skipper_lqi_for_export_P.A[12 * i + i_0] *
        skipper_lqi_for_export_B.state_with_perturbation[i];
    }

    skipper_lqi_for_export_B.state_gain[i_0] = state_gain;

    /* End of Gain: '<Root>/state_gain' */
  }

  /* Gain: '<Root>/extraction_gain' */
  for (i_0 = 0; i_0 <= 34; i_0 += 2) {
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_P.E[i_0]);
    tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(-1.0));
    _mm_storeu_pd(&tmp[i_0], tmp_4);
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    /* Gain: '<Root>/extraction_gain' */
    state_gain = 0.0;
    for (i = 0; i < 12; i++) {
      state_gain += tmp[3 * i + i_0] *
        skipper_lqi_for_export_B.state_with_perturbation[i];
    }

    skipper_lqi_for_export_B.extraction_gain[i_0] = state_gain;

    /* End of Gain: '<Root>/extraction_gain' */
  }

  /* Inport: '<Root>/refeference_in' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Sum: '<Root>/state_derivative_sum'
   */
  tmp_4 = _mm_add_pd(_mm_add_pd(_mm_loadu_pd
    (&skipper_lqi_for_export_U.reference[0]), _mm_loadu_pd
    (&skipper_lqi_for_export_B.extraction_gain[0])), _mm_loadu_pd
                     (&skipper_lqi_for_export_P.dummy_zeros_Value[0]));

  /* Sum: '<Root>/state_derivative_sum' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Inport: '<Root>/refeference_in'
   */
  _mm_storeu_pd(&skipper_lqi_for_export_B.state_with_error_derivative[12], tmp_4);
  skipper_lqi_for_export_B.state_with_error_derivative[14] =
    (skipper_lqi_for_export_U.reference[2] +
     skipper_lqi_for_export_B.extraction_gain[2]) +
    skipper_lqi_for_export_P.dummy_zeros_Value[2];
  for (i = 0; i <= 10; i += 2) {
    /* UnitDelay: '<Root>/Unit Delay1' */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i]);
    _mm_storeu_pd(&skipper_lqi_for_export_B.UnitDelay1[i], tmp_4);

    /* Sum: '<Root>/state_derivative_sum' incorporates:
     *  Constant: '<Root>/gravity_const'
     *  UnitDelay: '<Root>/Unit Delay1'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.d[i]);
    tmp_3 = _mm_loadu_pd(&skipper_lqi_for_export_B.state_gain[i]);
    tmp_5 = _mm_add_pd(tmp_5, tmp_3);
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Sum: '<Root>/state_derivative_sum' incorporates:
     *  UnitDelay: '<Root>/Unit Delay1'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_B.state_with_error_derivative[i],
                  tmp_4);

    /* Constant: '<Root>/initial_state' incorporates:
     *  UnitDelay: '<Root>/Unit Delay1'
     */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_P.initial_state_Value[i]);
    _mm_storeu_pd(&skipper_lqi_for_export_B.initial_state[i], tmp_4);
  }

  /* Constant: '<Root>/initial_error ' */
  skipper_lqi_for_export_B.initial_error[0] =
    skipper_lqi_for_export_P.initial_error_Value[0];
  skipper_lqi_for_export_B.initial_error[1] =
    skipper_lqi_for_export_P.initial_error_Value[1];
  skipper_lqi_for_export_B.initial_error[2] =
    skipper_lqi_for_export_P.initial_error_Value[2];
  for (i = 0; i <= 12; i += 2) {
    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_B.state_with_error_derivative[i]);
    tmp_4 = _mm_mul_pd(_mm_set1_pd
                       (skipper_lqi_for_export_P.DiscreteIntegrator_gainval),
                       tmp_4);
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i]);
    tmp_4 = _mm_add_pd(tmp_4, tmp_5);

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' */
    _mm_storeu_pd(&skipper_lqi_for_export_B.state_with_error[i], tmp_4);
  }

  for (i = 14; i < 15; i++) {
    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    skipper_lqi_for_export_B.state_with_error[i] =
      skipper_lqi_for_export_P.DiscreteIntegrator_gainval *
      skipper_lqi_for_export_B.state_with_error_derivative[i] +
      skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i];
  }

  for (i_0 = 0; i_0 < 12; i_0++) {
    /* Gain: '<Root>/output_gain' */
    state_gain = 0.0;
    for (i = 0; i < 12; i++) {
      /* Gain: '<Root>/output_gain' */
      state_gain += skipper_lqi_for_export_P.C[12 * i + i_0] *
        skipper_lqi_for_export_B.state_with_error[i];
    }

    /* Gain: '<Root>/output_gain' */
    skipper_lqi_for_export_B.output_gain[i_0] = state_gain;

    /* Gain: '<Root>/lqi_gain' */
    tmp_0[i_0] = -skipper_lqi_for_export_P.K2[i_0];
  }

  /* Gain: '<Root>/lqi_gain' */
  state_gain = skipper_lqi_for_export_B.state_with_error[12];
  state_with_error = skipper_lqi_for_export_B.state_with_error[13];
  state_with_error_0 = skipper_lqi_for_export_B.state_with_error[14];
  for (i_0 = 0; i_0 <= 2; i_0 += 2) {
    /* Gain: '<Root>/lqi_gain' */
    tmp_4 = _mm_loadu_pd(&tmp_0[i_0]);
    tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(state_gain));
    tmp_5 = _mm_loadu_pd(&tmp_0[i_0 + 4]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error));
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);
    tmp_5 = _mm_loadu_pd(&tmp_0[i_0 + 8]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error_0));
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Gain: '<Root>/lqi_gain' */
    _mm_storeu_pd(&skipper_lqi_for_export_B.fb_error_control[i_0], tmp_4);
  }

  /* Gain: '<Root>/lqr_gain' */
  for (i_0 = 0; i_0 <= 46; i_0 += 2) {
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_P.K1[i_0]);
    tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(-1.0));
    _mm_storeu_pd(&tmp_1[i_0], tmp_4);
  }

  for (i = 0; i < 4; i++) {
    /* Gain: '<Root>/lqr_gain' */
    state_gain = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      /* Gain: '<Root>/lqr_gain' */
      state_gain += tmp_1[(i_0 << 2) + i] *
        skipper_lqi_for_export_B.state_with_error[i_0];
    }

    /* Gain: '<Root>/lqr_gain' */
    skipper_lqi_for_export_B.fb_state_control[i] = state_gain;

    /* Sum: '<Root>/control_sum' incorporates:
     *  Constant: '<Root>/ff_control'
     *  Gain: '<Root>/lqi_gain'
     *  Gain: '<Root>/lqr_gain'
     */
    state_gain = (skipper_lqi_for_export_B.fb_error_control[i] + state_gain) +
      skipper_lqi_for_export_P.Uff[i];
    skipper_lqi_for_export_B.net_control[i] = state_gain;

    /* Saturate: '<S2>/saturator' */
    state_with_error = skipper_lqi_for_export_P.saturator_LowerSat[i];
    state_with_error_0 = skipper_lqi_for_export_P.saturator_UpperSat[i];
    if (state_gain > state_with_error_0) {
      state_gain = state_with_error_0;
    } else if (state_gain < state_with_error) {
      state_gain = state_with_error;
    }

    /* Saturate: '<S2>/saturator' */
    skipper_lqi_for_export_B.saturator[i] = state_gain;
  }

  /* RateLimiter: '<S2>/Rate Limiter3' incorporates:
   *  Outport: '<Root>/Out13'
   */
  state_gain = skipper_lqi_for_export_B.saturator[0] -
    skipper_lqi_for_export_DW.PrevY;
  if (state_gain > skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.thrust =
      skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY;
  } else if (state_gain < skipper_lqi_for_export_P.RateLimiter3_FallingLim *
             skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.thrust =
      skipper_lqi_for_export_P.RateLimiter3_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY;
  } else {
    skipper_lqi_for_export_Y.thrust = skipper_lqi_for_export_B.saturator[0];
  }

  skipper_lqi_for_export_DW.PrevY = skipper_lqi_for_export_Y.thrust;

  /* End of RateLimiter: '<S2>/Rate Limiter3' */

  /* RateLimiter: '<S2>/Rate Limiter2' incorporates:
   *  Outport: '<Root>/Out14'
   */
  state_gain = skipper_lqi_for_export_B.saturator[1] -
    skipper_lqi_for_export_DW.PrevY_k;
  if (state_gain > skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.reaction_torque =
      skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_k;
  } else if (state_gain < skipper_lqi_for_export_P.RateLimiter2_FallingLim *
             skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.reaction_torque =
      skipper_lqi_for_export_P.RateLimiter2_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_k;
  } else {
    skipper_lqi_for_export_Y.reaction_torque =
      skipper_lqi_for_export_B.saturator[1];
  }

  skipper_lqi_for_export_DW.PrevY_k = skipper_lqi_for_export_Y.reaction_torque;

  /* End of RateLimiter: '<S2>/Rate Limiter2' */

  /* RateLimiter: '<S2>/Rate Limiter1' */
  state_gain = skipper_lqi_for_export_B.saturator[2] -
    skipper_lqi_for_export_DW.PrevY_b;
  if (state_gain > skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter1' */
    skipper_lqi_for_export_B.upper_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_b;
  } else if (state_gain < skipper_lqi_for_export_P.RateLimiter1_FallingLim *
             skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter1' */
    skipper_lqi_for_export_B.upper_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter1_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_b;
  } else {
    /* RateLimiter: '<S2>/Rate Limiter1' */
    skipper_lqi_for_export_B.upper_gimbal_angle =
      skipper_lqi_for_export_B.saturator[2];
  }

  skipper_lqi_for_export_DW.PrevY_b =
    skipper_lqi_for_export_B.upper_gimbal_angle;

  /* End of RateLimiter: '<S2>/Rate Limiter1' */

  /* RateLimiter: '<S2>/Rate Limiter' */
  state_gain = skipper_lqi_for_export_B.saturator[3] -
    skipper_lqi_for_export_DW.PrevY_l;
  if (state_gain > skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter' */
    skipper_lqi_for_export_B.lower_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_l;
  } else if (state_gain < skipper_lqi_for_export_P.RateLimiter_FallingLim *
             skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter' */
    skipper_lqi_for_export_B.lower_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_l;
  } else {
    /* RateLimiter: '<S2>/Rate Limiter' */
    skipper_lqi_for_export_B.lower_gimbal_angle =
      skipper_lqi_for_export_B.saturator[3];
  }

  skipper_lqi_for_export_DW.PrevY_l =
    skipper_lqi_for_export_B.lower_gimbal_angle;

  /* End of RateLimiter: '<S2>/Rate Limiter' */

  /* Outport: '<Root>/control_commands_out' incorporates:
   *  Outport: '<Root>/Out13'
   *  Outport: '<Root>/Out14'
   *  SignalConversion generated from: '<Root>/input_gain'
   */
  skipper_lqi_for_export_Y.control[0] = skipper_lqi_for_export_Y.thrust;
  skipper_lqi_for_export_Y.control[1] = skipper_lqi_for_export_Y.reaction_torque;
  skipper_lqi_for_export_Y.control[2] =
    skipper_lqi_for_export_B.upper_gimbal_angle;
  skipper_lqi_for_export_Y.control[3] =
    skipper_lqi_for_export_B.lower_gimbal_angle;

  /* Gain: '<Root>/passthrough_gain' incorporates:
   *  Outport: '<Root>/control_commands_out'
   */
  state_gain = skipper_lqi_for_export_Y.control[0];
  state_with_error = skipper_lqi_for_export_Y.control[1];
  state_with_error_0 = skipper_lqi_for_export_Y.control[2];
  control = skipper_lqi_for_export_Y.control[3];
  for (i = 0; i <= 10; i += 2) {
    /* Gain: '<Root>/passthrough_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_P.D[i]);
    tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(state_gain));
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.D[i + 12]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error));

    /* Gain: '<Root>/passthrough_gain' */
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Gain: '<Root>/passthrough_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.D[i + 24]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error_0));

    /* Gain: '<Root>/passthrough_gain' */
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Gain: '<Root>/passthrough_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.D[i + 36]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(control));

    /* Gain: '<Root>/passthrough_gain' */
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);
    _mm_storeu_pd(&skipper_lqi_for_export_B.passthrough_gain[i], tmp_4);

    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/output_gain'
     *  Gain: '<Root>/passthrough_gain'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.output_gain[i]);
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/passthrough_gain'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_B.output[i], tmp_4);
  }

  /* Outport: '<Root>/Out1' */
  skipper_lqi_for_export_Y.altitude = skipper_lqi_for_export_B.output[0];

  /* Outport: '<Root>/Out2' */
  skipper_lqi_for_export_Y.crossrange = skipper_lqi_for_export_B.output[1];

  /* Outport: '<Root>/Out3' */
  skipper_lqi_for_export_Y.downrange = skipper_lqi_for_export_B.output[2];

  /* Outport: '<Root>/Out4' */
  skipper_lqi_for_export_Y.lateral_vel = skipper_lqi_for_export_B.output[3];

  /* Outport: '<Root>/Out5' */
  skipper_lqi_for_export_Y.longitudinal_vel = skipper_lqi_for_export_B.output[4];

  /* Outport: '<Root>/Out6' */
  skipper_lqi_for_export_Y.directional_vel = skipper_lqi_for_export_B.output[5];

  /* Gain: '<Root>/Gain11' incorporates:
   *  Gain: '<Root>/Gain12'
   */
  tmp_4 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain11_Gain,
    skipper_lqi_for_export_P.Gain12_Gain), _mm_loadu_pd
                     (&skipper_lqi_for_export_B.output[6]));
  _mm_storeu_pd(&tmp_6[0], tmp_4);

  /* Outport: '<Root>/Out7' */
  skipper_lqi_for_export_Y.roll = tmp_6[0];

  /* Outport: '<Root>/Out8' */
  skipper_lqi_for_export_Y.pitch = tmp_6[1];

  /* Gain: '<Root>/Gain9' incorporates:
   *  Gain: '<Root>/Gain10'
   */
  tmp_4 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain9_Gain,
    skipper_lqi_for_export_P.Gain10_Gain), _mm_loadu_pd
                     (&skipper_lqi_for_export_B.output[8]));
  _mm_storeu_pd(&tmp_6[0], tmp_4);

  /* Outport: '<Root>/Out9' */
  skipper_lqi_for_export_Y.yaw = tmp_6[0];

  /* Outport: '<Root>/Out10' */
  skipper_lqi_for_export_Y.lateral_rate = tmp_6[1];

  /* Gain: '<Root>/Gain7' incorporates:
   *  Gain: '<Root>/Gain8'
   */
  tmp_4 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain7_Gain,
    skipper_lqi_for_export_P.Gain8_Gain), _mm_loadu_pd
                     (&skipper_lqi_for_export_B.output[10]));
  _mm_storeu_pd(&tmp_6[0], tmp_4);

  /* Outport: '<Root>/Out11' */
  skipper_lqi_for_export_Y.longitudinal_rate = tmp_6[0];

  /* Outport: '<Root>/Out12' */
  skipper_lqi_for_export_Y.directional_rate = tmp_6[1];

  /* Outport: '<Root>/Out15' incorporates:
   *  Gain: '<Root>/Gain5'
   */
  skipper_lqi_for_export_Y.upper_gimbal_angle =
    skipper_lqi_for_export_P.Gain5_Gain *
    skipper_lqi_for_export_B.upper_gimbal_angle;

  /* Outport: '<Root>/Out16' incorporates:
   *  Gain: '<Root>/Gain6'
   */
  skipper_lqi_for_export_Y.lower_gimbal_angle =
    skipper_lqi_for_export_P.Gain6_Gain *
    skipper_lqi_for_export_B.lower_gimbal_angle;

  /* Outport: '<Root>/Out17' */
  skipper_lqi_for_export_Y.altitude_error =
    skipper_lqi_for_export_B.state_with_error[12];

  /* Outport: '<Root>/Out18' */
  skipper_lqi_for_export_Y.crossrange_error =
    skipper_lqi_for_export_B.state_with_error[13];

  /* Outport: '<Root>/Out19' */
  skipper_lqi_for_export_Y.downrange_error =
    skipper_lqi_for_export_B.state_with_error[14];

  /* MATLAB Function: '<S52>/SqrtUsedFcn' incorporates:
   *  Constant: '<S3>/CovarianceZ'
   *  Constant: '<S52>/isSqrtUsed'
   */
  /* :  if isSqrtUsed */
  if (skipper_lqi_for_export_P.isSqrtUsed_Value) {
    /* :  P = u*u.'; */
    for (i_0 = 0; i_0 < 12; i_0++) {
      for (i = 0; i < 12; i++) {
        state_gain = 0.0;
        for (i_1 = 0; i_1 < 12; i_1++) {
          state_gain += skipper_lqi_for_export_P.CovarianceZ_Value[12 * i_1 +
            i_0] * skipper_lqi_for_export_P.CovarianceZ_Value[12 * i_1 + i];
        }

        skipper_lqi_for_export_B.P[i_0 + 12 * i] = state_gain;
      }
    }
  } else {
    /* :  else */
    /* :  P = u; */
    std::memcpy(&skipper_lqi_for_export_B.P[0],
                &skipper_lqi_for_export_P.CovarianceZ_Value[0], 144U * sizeof
                (real_T));
  }

  /* End of MATLAB Function: '<S52>/SqrtUsedFcn' */
  for (i = 0; i < 12; i++) {
    /* Delay: '<S1>/MemoryX' incorporates:
     *  Constant: '<S1>/X0'
     */
    if (skipper_lqi_for_export_DW.icLoad) {
      skipper_lqi_for_export_DW.MemoryX_DSTATE[i] =
        skipper_lqi_for_export_P.X0_Value[i];
    }

    /* Delay: '<S1>/MemoryX' */
    skipper_lqi_for_export_B.MemoryX[i] =
      skipper_lqi_for_export_DW.MemoryX_DSTATE[i];
  }

  /* Outputs for Enabled SubSystem: '<S30>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S56>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S23>/MeasurementUpdate' incorporates:
   *  EnablePort: '<S54>/Enable'
   */
  /* Product: '<S23>/A[k]*xhat[k|k-1]' incorporates:
   *  Constant: '<S1>/A'
   *  Product: '<S54>/C[k]*xhat[k|k-1]'
   *  Product: '<S54>/Product3'
   *  Product: '<S56>/Product'
   *  Product: '<S56>/Product2'
   */
  tmp_7 = &skipper_lqi_for_export_P.A_Value[0];

  /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
  /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
  std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
              (real_T));
  for (i_0 = 0; i_0 < 12; i_0++) {
    state_gain = 0.0;
    for (i = 0; i < 12; i++) {
      state_gain += tmp_7[12 * i + i_0] * tmp_0[i];
    }

    /* Product: '<S23>/A[k]*xhat[k|k-1]' */
    skipper_lqi_for_export_B.Akxhatkk1[i_0] = state_gain;
  }

  /* Reshape: '<S1>/Reshapeu' incorporates:
   *  Outport: '<Root>/control_commands_out'
   */
  skipper_lqi_for_export_B.Reshapeu[0] = skipper_lqi_for_export_Y.control[0];
  skipper_lqi_for_export_B.Reshapeu[1] = skipper_lqi_for_export_Y.control[1];
  skipper_lqi_for_export_B.Reshapeu[2] = skipper_lqi_for_export_Y.control[2];
  skipper_lqi_for_export_B.Reshapeu[3] = skipper_lqi_for_export_Y.control[3];

  /* Product: '<S23>/B[k]*u[k]' incorporates:
   *  Constant: '<S1>/B'
   */
  std::memcpy(&tmp_1[0], &skipper_lqi_for_export_P.B_Value[0], 48U * sizeof
              (real_T));
  state_with_error = skipper_lqi_for_export_B.Reshapeu[0];
  state_with_error_0 = skipper_lqi_for_export_B.Reshapeu[1];
  control = skipper_lqi_for_export_B.Reshapeu[2];
  tmp_2 = skipper_lqi_for_export_B.Reshapeu[3];
  for (i_0 = 0; i_0 <= 10; i_0 += 2) {
    tmp_4 = _mm_loadu_pd(&tmp_1[i_0]);
    tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(state_with_error));
    tmp_5 = _mm_loadu_pd(&tmp_1[i_0 + 12]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error_0));
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);
    tmp_5 = _mm_loadu_pd(&tmp_1[i_0 + 24]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(control));
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);
    tmp_5 = _mm_loadu_pd(&tmp_1[i_0 + 36]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(tmp_2));
    tmp_4 = _mm_add_pd(tmp_5, tmp_4);

    /* Product: '<S23>/B[k]*u[k]' */
    _mm_storeu_pd(&skipper_lqi_for_export_B.Bkuk[i_0], tmp_4);
  }

  /* End of Product: '<S23>/B[k]*u[k]' */

  /* Outputs for Enabled SubSystem: '<S23>/MeasurementUpdate' incorporates:
   *  EnablePort: '<S54>/Enable'
   */
  /* Constant: '<S1>/Enable' */
  if (skipper_lqi_for_export_P.Enable_Value) {
    skipper_lqi_for_export_DW.MeasurementUpdate_MODE = true;

    /* Outputs for Enabled SubSystem: '<S30>/Enabled Subsystem' incorporates:
     *  EnablePort: '<S56>/Enable'
     */
    /* Product: '<S54>/C[k]*xhat[k|k-1]' incorporates:
     *  Constant: '<S1>/C'
     *  Product: '<S23>/A[k]*xhat[k|k-1]'
     *  Product: '<S54>/Product3'
     *  Product: '<S56>/Product'
     *  Product: '<S56>/Product2'
     */
    tmp_7 = &skipper_lqi_for_export_P.C_Value[0];

    /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
                (real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      state_gain = 0.0;
      for (i = 0; i < 12; i++) {
        state_gain += tmp_7[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S54>/C[k]*xhat[k|k-1]' */
      skipper_lqi_for_export_B.Ckxhatkk1[i_0] = state_gain;
    }

    /* Product: '<S54>/D[k]*u[k]' incorporates:
     *  Constant: '<S1>/D'
     */
    std::memcpy(&tmp_1[0], &skipper_lqi_for_export_P.D_Value[0], 48U * sizeof
                (real_T));
    for (i = 0; i <= 10; i += 2) {
      tmp_4 = _mm_loadu_pd(&tmp_1[i]);
      tmp_4 = _mm_mul_pd(tmp_4, _mm_set1_pd(state_with_error));
      tmp_5 = _mm_loadu_pd(&tmp_1[i + 12]);
      tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_with_error_0));
      tmp_4 = _mm_add_pd(tmp_5, tmp_4);
      tmp_5 = _mm_loadu_pd(&tmp_1[i + 24]);
      tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(control));
      tmp_4 = _mm_add_pd(tmp_5, tmp_4);
      tmp_5 = _mm_loadu_pd(&tmp_1[i + 36]);
      tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(tmp_2));
      tmp_4 = _mm_add_pd(tmp_5, tmp_4);

      /* Product: '<S54>/D[k]*u[k]' */
      _mm_storeu_pd(&skipper_lqi_for_export_B.Dkuk[i], tmp_4);

      /* Sum: '<S54>/Add1' */
      tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.Ckxhatkk1[i]);
      tmp_4 = _mm_add_pd(tmp_5, tmp_4);

      /* Sum: '<S54>/Add1' */
      _mm_storeu_pd(&skipper_lqi_for_export_B.yhatkk1[i], tmp_4);

      /* Sum: '<S54>/Sum' incorporates:
       *  Inport: '<Root>/imu_state_in'
       *  Sum: '<S54>/Add1'
       */
      tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_U.imu_state_in[i]);
      tmp_4 = _mm_sub_pd(tmp_5, tmp_4);

      /* Sum: '<S54>/Sum' */
      _mm_storeu_pd(&skipper_lqi_for_export_B.Sum[i], tmp_4);
    }

    /* End of Product: '<S54>/D[k]*u[k]' */

    /* Outputs for Enabled SubSystem: '<S30>/Enabled Subsystem' incorporates:
     *  EnablePort: '<S56>/Enable'
     */
    /* Product: '<S54>/Product3' incorporates:
     *  Constant: '<S3>/KalmanGainL'
     *  Product: '<S23>/A[k]*xhat[k|k-1]'
     *  Product: '<S54>/C[k]*xhat[k|k-1]'
     *  Product: '<S56>/Product'
     *  Product: '<S56>/Product2'
     */
    tmp_7 = &skipper_lqi_for_export_P.KalmanGainL_Value[0];

    /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.Sum[0], 12U * sizeof(real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      state_gain = 0.0;
      for (i = 0; i < 12; i++) {
        state_gain += tmp_7[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S54>/Product3' */
      skipper_lqi_for_export_B.Product3[i_0] = state_gain;
    }
  } else if (skipper_lqi_for_export_DW.MeasurementUpdate_MODE) {
    for (i = 0; i < 12; i++) {
      /* Disable for Product: '<S54>/Product3' incorporates:
       *  Outport: '<S54>/L*(y[k]-yhat[k|k-1])'
       */
      skipper_lqi_for_export_B.Product3[i] =
        skipper_lqi_for_export_P.Lykyhatkk1_Y0;
    }

    skipper_lqi_for_export_DW.MeasurementUpdate_MODE = false;
  }

  /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
  for (i = 0; i <= 10; i += 2) {
    /* Sum: '<S23>/Add' */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_B.Bkuk[i]);
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.Akxhatkk1[i]);
    tmp_4 = _mm_add_pd(tmp_4, tmp_5);
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.Product3[i]);
    tmp_4 = _mm_add_pd(tmp_4, tmp_5);

    /* Sum: '<S23>/Add' */
    _mm_storeu_pd(&skipper_lqi_for_export_B.Add[i], tmp_4);
  }

  /* Outputs for Enabled SubSystem: '<S30>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S56>/Enable'
   */
  /* Constant: '<S1>/Enable' */
  if (skipper_lqi_for_export_P.Enable_Value) {
    skipper_lqi_for_export_DW.EnabledSubsystem_MODE = true;

    /* Outputs for Enabled SubSystem: '<S23>/MeasurementUpdate' incorporates:
     *  EnablePort: '<S54>/Enable'
     */
    /* Product: '<S56>/Product' incorporates:
     *  Constant: '<S1>/C'
     *  Product: '<S23>/A[k]*xhat[k|k-1]'
     *  Product: '<S54>/C[k]*xhat[k|k-1]'
     *  Product: '<S54>/Product3'
     *  Product: '<S56>/Product2'
     */
    tmp_7 = &skipper_lqi_for_export_P.C_Value[0];

    /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
                (real_T));
    for (i = 0; i < 12; i++) {
      state_gain = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        state_gain += tmp_7[12 * i_0 + i] * tmp_0[i_0];
      }

      /* Product: '<S56>/Product' */
      skipper_lqi_for_export_B.Product[i] = state_gain;

      /* Sum: '<S56>/Add1' incorporates:
       *  Inport: '<Root>/imu_state_in'
       */
      skipper_lqi_for_export_B.Add1[i] = skipper_lqi_for_export_U.imu_state_in[i]
        - state_gain;
    }

    /* Outputs for Enabled SubSystem: '<S23>/MeasurementUpdate' incorporates:
     *  EnablePort: '<S54>/Enable'
     */
    /* Product: '<S56>/Product2' incorporates:
     *  Constant: '<S3>/KalmanGainM'
     *  Product: '<S23>/A[k]*xhat[k|k-1]'
     *  Product: '<S54>/C[k]*xhat[k|k-1]'
     *  Product: '<S54>/Product3'
     *  Product: '<S56>/Product'
     */
    tmp_7 = &skipper_lqi_for_export_P.KalmanGainM_Value[0];

    /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.Add1[0], 12U * sizeof
                (real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      state_gain = 0.0;
      for (i = 0; i < 12; i++) {
        state_gain += tmp_7[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S56>/Product2' */
      skipper_lqi_for_export_B.Product2[i_0] = state_gain;
    }
  } else if (skipper_lqi_for_export_DW.EnabledSubsystem_MODE) {
    for (i = 0; i < 12; i++) {
      /* Disable for Product: '<S56>/Product2' incorporates:
       *  Outport: '<S56>/deltax'
       */
      skipper_lqi_for_export_B.Product2[i] = skipper_lqi_for_export_P.deltax_Y0;
    }

    skipper_lqi_for_export_DW.EnabledSubsystem_MODE = false;
  }

  /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */

  /* Gain: '<Root>/input_gain' incorporates:
   *  Outport: '<Root>/control_commands_out'
   */
  state_gain = skipper_lqi_for_export_Y.control[0];
  state_with_error = skipper_lqi_for_export_Y.control[1];
  state_with_error_0 = skipper_lqi_for_export_Y.control[2];
  control = skipper_lqi_for_export_Y.control[3];
  for (i = 0; i <= 10; i += 2) {
    /* Sum: '<S30>/Add' */
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_B.Product2[i]);
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.MemoryX[i]);
    tmp_4 = _mm_add_pd(tmp_4, tmp_5);

    /* Sum: '<S30>/Add' */
    _mm_storeu_pd(&skipper_lqi_for_export_B.Add_j[i], tmp_4);

    /* Gain: '<Root>/input_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     *  Sum: '<S30>/Add'
     */
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_P.B[i]);
    tmp_5 = _mm_mul_pd(tmp_5, _mm_set1_pd(state_gain));
    tmp_3 = _mm_loadu_pd(&skipper_lqi_for_export_P.B[i + 12]);
    tmp_3 = _mm_mul_pd(tmp_3, _mm_set1_pd(state_with_error));

    /* Gain: '<Root>/input_gain' */
    tmp_5 = _mm_add_pd(tmp_3, tmp_5);

    /* Gain: '<Root>/input_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     *  Sum: '<S30>/Add'
     */
    tmp_3 = _mm_loadu_pd(&skipper_lqi_for_export_P.B[i + 24]);
    tmp_3 = _mm_mul_pd(tmp_3, _mm_set1_pd(state_with_error_0));

    /* Gain: '<Root>/input_gain' */
    tmp_5 = _mm_add_pd(tmp_3, tmp_5);

    /* Gain: '<Root>/input_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     *  Sum: '<S30>/Add'
     */
    tmp_3 = _mm_loadu_pd(&skipper_lqi_for_export_P.B[i + 36]);
    tmp_3 = _mm_mul_pd(tmp_3, _mm_set1_pd(control));

    /* Gain: '<Root>/input_gain' incorporates:
     *  Sum: '<S30>/Add'
     */
    tmp_5 = _mm_add_pd(tmp_3, tmp_5);
    _mm_storeu_pd(&skipper_lqi_for_export_B.input_gain[i], tmp_5);

    /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
     *  Sum: '<S30>/Add'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_DW.UnitDelay_DSTATE[i], tmp_4);

    /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
     *  Gain: '<Root>/input_gain'
     *  Sum: '<S30>/Add'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i], tmp_5);
  }

  /* Update for DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
   *  Sum: '<Root>/state_derivative_sum'
   */
  for (i = 0; i <= 12; i += 2) {
    tmp_4 = _mm_loadu_pd(&skipper_lqi_for_export_B.state_with_error_derivative[i]);
    tmp_4 = _mm_mul_pd(_mm_set1_pd
                       (skipper_lqi_for_export_P.DiscreteIntegrator_gainval),
                       tmp_4);
    tmp_5 = _mm_loadu_pd(&skipper_lqi_for_export_B.state_with_error[i]);
    tmp_4 = _mm_add_pd(tmp_4, tmp_5);
    _mm_storeu_pd(&skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i], tmp_4);
  }

  for (i = 14; i < 15; i++) {
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i] =
      skipper_lqi_for_export_P.DiscreteIntegrator_gainval *
      skipper_lqi_for_export_B.state_with_error_derivative[i] +
      skipper_lqi_for_export_B.state_with_error[i];
  }

  /* End of Update for DiscreteIntegrator: '<Root>/Discrete Integrator' */

  /* Update for Delay: '<S1>/MemoryX' */
  skipper_lqi_for_export_DW.icLoad = false;
  std::memcpy(&skipper_lqi_for_export_DW.MemoryX_DSTATE[0],
              &skipper_lqi_for_export_B.Add[0], 12U * sizeof(real_T));

  /* Matfile logging */
  rt_UpdateTXYLogVars((&skipper_lqi_for_export_M)->rtwLogInfo,
                      (&(&skipper_lqi_for_export_M)->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.05s, 0.0s] */
    if ((rtmGetTFinal((&skipper_lqi_for_export_M))!=-1) &&
        !((rtmGetTFinal((&skipper_lqi_for_export_M))-(&skipper_lqi_for_export_M
           )->Timing.taskTime0) > (&skipper_lqi_for_export_M)->Timing.taskTime0 *
          (DBL_EPSILON))) {
      rtmSetErrorStatus((&skipper_lqi_for_export_M), "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++(&skipper_lqi_for_export_M)->Timing.clockTick0)) {
    ++(&skipper_lqi_for_export_M)->Timing.clockTickH0;
  }

  (&skipper_lqi_for_export_M)->Timing.taskTime0 = (&skipper_lqi_for_export_M)
    ->Timing.clockTick0 * (&skipper_lqi_for_export_M)->Timing.stepSize0 +
    (&skipper_lqi_for_export_M)->Timing.clockTickH0 * (&skipper_lqi_for_export_M)
    ->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void skipper_lqi_for_export::initialize()
{
  /* Registration code */
  rtmSetTFinal((&skipper_lqi_for_export_M), 100.0);
  (&skipper_lqi_for_export_M)->Timing.stepSize0 = 0.05;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (nullptr);
    (&skipper_lqi_for_export_M)->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo((&skipper_lqi_for_export_M)->rtwLogInfo, (nullptr));
    rtliSetLogXSignalPtrs((&skipper_lqi_for_export_M)->rtwLogInfo, (nullptr));
    rtliSetLogT((&skipper_lqi_for_export_M)->rtwLogInfo, "tout");
    rtliSetLogX((&skipper_lqi_for_export_M)->rtwLogInfo, "");
    rtliSetLogXFinal((&skipper_lqi_for_export_M)->rtwLogInfo, "");
    rtliSetLogVarNameModifier((&skipper_lqi_for_export_M)->rtwLogInfo, "rt_");
    rtliSetLogFormat((&skipper_lqi_for_export_M)->rtwLogInfo, 4);
    rtliSetLogMaxRows((&skipper_lqi_for_export_M)->rtwLogInfo, 0);
    rtliSetLogDecimation((&skipper_lqi_for_export_M)->rtwLogInfo, 1);
    rtliSetLogY((&skipper_lqi_for_export_M)->rtwLogInfo, "");
    rtliSetLogYSignalInfo((&skipper_lqi_for_export_M)->rtwLogInfo, (nullptr));
    rtliSetLogYSignalPtrs((&skipper_lqi_for_export_M)->rtwLogInfo, (nullptr));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime((&skipper_lqi_for_export_M)->rtwLogInfo, 0.0,
    rtmGetTFinal((&skipper_lqi_for_export_M)), (&skipper_lqi_for_export_M)
    ->Timing.stepSize0, (&rtmGetErrorStatus((&skipper_lqi_for_export_M))));

  /* Start for Constant: '<Root>/initial_state' */
  std::memcpy(&skipper_lqi_for_export_B.initial_state[0],
              &skipper_lqi_for_export_P.initial_state_Value[0], 12U * sizeof
              (real_T));

  /* Start for Constant: '<Root>/initial_error ' */
  skipper_lqi_for_export_B.initial_error[0] =
    skipper_lqi_for_export_P.initial_error_Value[0];
  skipper_lqi_for_export_B.initial_error[1] =
    skipper_lqi_for_export_P.initial_error_Value[1];
  skipper_lqi_for_export_B.initial_error[2] =
    skipper_lqi_for_export_P.initial_error_Value[2];

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' */
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[12] =
    skipper_lqi_for_export_B.initial_error[0];
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[13] =
    skipper_lqi_for_export_B.initial_error[1];
  skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[14] =
    skipper_lqi_for_export_B.initial_error[2];

  /* InitializeConditions for RateLimiter: '<S2>/Rate Limiter3' */
  skipper_lqi_for_export_DW.PrevY = skipper_lqi_for_export_P.RateLimiter3_IC;

  /* InitializeConditions for RateLimiter: '<S2>/Rate Limiter2' */
  skipper_lqi_for_export_DW.PrevY_k = skipper_lqi_for_export_P.RateLimiter2_IC;

  /* InitializeConditions for RateLimiter: '<S2>/Rate Limiter1' */
  skipper_lqi_for_export_DW.PrevY_b = skipper_lqi_for_export_P.RateLimiter1_IC;

  /* InitializeConditions for RateLimiter: '<S2>/Rate Limiter' */
  skipper_lqi_for_export_DW.PrevY_l = skipper_lqi_for_export_P.RateLimiter_IC;

  /* InitializeConditions for Delay: '<S1>/MemoryX' */
  skipper_lqi_for_export_DW.icLoad = true;

  /* SystemInitialize for Enabled SubSystem: '<S30>/Enabled Subsystem' */
  /* SystemInitialize for Enabled SubSystem: '<S23>/MeasurementUpdate' */
  for (int32_T i{0}; i < 12; i++) {
    /* InitializeConditions for UnitDelay: '<Root>/Unit Delay' */
    skipper_lqi_for_export_DW.UnitDelay_DSTATE[i] =
      skipper_lqi_for_export_P.UnitDelay_InitialCondition;

    /* InitializeConditions for UnitDelay: '<Root>/Unit Delay1' */
    skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i] =
      skipper_lqi_for_export_P.UnitDelay1_InitialCondition;

    /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' */
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i] =
      skipper_lqi_for_export_B.initial_state[i];

    /* SystemInitialize for Product: '<S54>/Product3' incorporates:
     *  Outport: '<S54>/L*(y[k]-yhat[k|k-1])'
     */
    skipper_lqi_for_export_B.Product3[i] =
      skipper_lqi_for_export_P.Lykyhatkk1_Y0;

    /* SystemInitialize for Product: '<S56>/Product2' incorporates:
     *  Outport: '<S56>/deltax'
     */
    skipper_lqi_for_export_B.Product2[i] = skipper_lqi_for_export_P.deltax_Y0;
  }

  /* End of SystemInitialize for SubSystem: '<S23>/MeasurementUpdate' */
  /* End of SystemInitialize for SubSystem: '<S30>/Enabled Subsystem' */
}

/* Model terminate function */
void skipper_lqi_for_export::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
skipper_lqi_for_export::skipper_lqi_for_export() :
  skipper_lqi_for_export_U(),
  skipper_lqi_for_export_Y(),
  skipper_lqi_for_export_B(),
  skipper_lqi_for_export_DW(),
  skipper_lqi_for_export_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
/* Currently there is no destructor body generated.*/
skipper_lqi_for_export::~skipper_lqi_for_export() = default;

/* Real-Time Model get method */
RT_MODEL_skipper_lqi_for_expo_T * skipper_lqi_for_export::getRTM()
{
  return (&skipper_lqi_for_export_M);
}
