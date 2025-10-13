/*
 * skipper_lqi_for_export.cpp
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi_for_export".
 *
 * Model version              : 3.4
 * Simulink Coder version : 24.1 (R2024a) 19-Nov-2023
 * C++ source code generated on : Mon Oct 13 17:03:39 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM 7
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "skipper_lqi_for_export.h"
#include "rtwtypes.h"
#include <cstring>

const real_T skipper_lqi_for_export_period{ 0.05 };

/* Model step function */
void skipper_lqi_for_export::step()
{
  real_T tmp_1[48];
  real_T tmp[36];
  real_T tmp_0[12];
  const real_T *tmp_2;
  real_T UnitDelay;
  real_T fb_error_control;
  real_T input_gain;
  real_T passthrough_gain;
  real_T state_with_error;
  real_T state_with_error_0;
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  for (i = 0; i < 12; i++) {
    /* UnitDelay: '<Root>/Unit Delay' */
    UnitDelay = skipper_lqi_for_export_DW.UnitDelay_DSTATE[i];
    skipper_lqi_for_export_B.UnitDelay[i] = UnitDelay;

    /* Sum: '<Root>/feedback_sum' incorporates:
     *  Constant: '<Root>/perturbation'
     */
    skipper_lqi_for_export_B.state_with_perturbation[i] =
      skipper_lqi_for_export_P.perturbation_Value[i] + UnitDelay;
  }

  for (i_0 = 0; i_0 < 12; i_0++) {
    /* Gain: '<Root>/state_gain' */
    UnitDelay = 0.0;
    for (i = 0; i < 12; i++) {
      UnitDelay += skipper_lqi_for_export_P.A[12 * i + i_0] *
        skipper_lqi_for_export_B.state_with_perturbation[i];
    }

    skipper_lqi_for_export_B.state_gain[i_0] = UnitDelay;

    /* End of Gain: '<Root>/state_gain' */
  }

  /* Gain: '<Root>/extraction_gain' */
  for (i_0 = 0; i_0 < 36; i_0++) {
    tmp[i_0] = -skipper_lqi_for_export_P.E[i_0];
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    /* Gain: '<Root>/extraction_gain' */
    UnitDelay = 0.0;
    for (i = 0; i < 12; i++) {
      UnitDelay += tmp[3 * i + i_0] *
        skipper_lqi_for_export_B.state_with_perturbation[i];
    }

    skipper_lqi_for_export_B.extraction_gain[i_0] = UnitDelay;

    /* End of Gain: '<Root>/extraction_gain' */
  }

  /* Sum: '<Root>/state_derivative_sum' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Inport: '<Root>/refeference_in'
   */
  skipper_lqi_for_export_B.state_with_error_derivative[12] =
    (skipper_lqi_for_export_U.reference[0] +
     skipper_lqi_for_export_B.extraction_gain[0]) +
    skipper_lqi_for_export_P.dummy_zeros_Value[0];
  skipper_lqi_for_export_B.state_with_error_derivative[13] =
    (skipper_lqi_for_export_U.reference[1] +
     skipper_lqi_for_export_B.extraction_gain[1]) +
    skipper_lqi_for_export_P.dummy_zeros_Value[1];
  skipper_lqi_for_export_B.state_with_error_derivative[14] =
    (skipper_lqi_for_export_U.reference[2] +
     skipper_lqi_for_export_B.extraction_gain[2]) +
    skipper_lqi_for_export_P.dummy_zeros_Value[2];
  for (i = 0; i < 12; i++) {
    /* UnitDelay: '<Root>/Unit Delay1' */
    UnitDelay = skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i];
    skipper_lqi_for_export_B.UnitDelay1[i] = UnitDelay;

    /* Sum: '<Root>/state_derivative_sum' incorporates:
     *  Constant: '<Root>/gravity_const'
     */
    skipper_lqi_for_export_B.state_with_error_derivative[i] =
      (skipper_lqi_for_export_P.d[i] + skipper_lqi_for_export_B.state_gain[i]) +
      UnitDelay;

    /* Constant: '<Root>/initial_state' */
    skipper_lqi_for_export_B.initial_state[i] =
      skipper_lqi_for_export_P.initial_state_Value[i];
  }

  /* Constant: '<Root>/initial_error ' */
  skipper_lqi_for_export_B.initial_error[0] =
    skipper_lqi_for_export_P.initial_error_Value[0];
  skipper_lqi_for_export_B.initial_error[1] =
    skipper_lqi_for_export_P.initial_error_Value[1];
  skipper_lqi_for_export_B.initial_error[2] =
    skipper_lqi_for_export_P.initial_error_Value[2];
  for (i = 0; i < 15; i++) {
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
    UnitDelay = 0.0;
    for (i = 0; i < 12; i++) {
      /* Gain: '<Root>/output_gain' */
      UnitDelay += skipper_lqi_for_export_P.C[12 * i + i_0] *
        skipper_lqi_for_export_B.state_with_error[i];
    }

    /* Gain: '<Root>/output_gain' */
    skipper_lqi_for_export_B.output_gain[i_0] = UnitDelay;

    /* Gain: '<Root>/lqi_gain' */
    tmp_0[i_0] = -skipper_lqi_for_export_P.K2[i_0];
  }

  /* Gain: '<Root>/lqi_gain' */
  UnitDelay = skipper_lqi_for_export_B.state_with_error[12];
  state_with_error = skipper_lqi_for_export_B.state_with_error[13];
  state_with_error_0 = skipper_lqi_for_export_B.state_with_error[14];
  for (i_0 = 0; i_0 < 4; i_0++) {
    /* Gain: '<Root>/lqi_gain' */
    fb_error_control = tmp_0[i_0] * UnitDelay;
    fb_error_control += tmp_0[i_0 + 4] * state_with_error;
    fb_error_control += tmp_0[i_0 + 8] * state_with_error_0;
    skipper_lqi_for_export_B.fb_error_control[i_0] = fb_error_control;
  }

  /* Gain: '<Root>/lqr_gain' */
  for (i_0 = 0; i_0 < 48; i_0++) {
    tmp_1[i_0] = -skipper_lqi_for_export_P.K1[i_0];
  }

  for (i = 0; i < 4; i++) {
    /* Gain: '<Root>/lqr_gain' */
    UnitDelay = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      /* Gain: '<Root>/lqr_gain' */
      UnitDelay += tmp_1[(i_0 << 2) + i] *
        skipper_lqi_for_export_B.state_with_error[i_0];
    }

    /* Gain: '<Root>/lqr_gain' */
    skipper_lqi_for_export_B.fb_state_control[i] = UnitDelay;

    /* Sum: '<Root>/control_sum' incorporates:
     *  Constant: '<Root>/ff_control'
     *  Gain: '<Root>/lqi_gain'
     *  Gain: '<Root>/lqr_gain'
     */
    UnitDelay = (skipper_lqi_for_export_B.fb_error_control[i] + UnitDelay) +
      skipper_lqi_for_export_P.Uff[i];
    skipper_lqi_for_export_B.net_control[i] = UnitDelay;

    /* Saturate: '<S2>/saturator' */
    state_with_error = skipper_lqi_for_export_P.saturator_LowerSat[i];
    state_with_error_0 = skipper_lqi_for_export_P.saturator_UpperSat[i];
    if (UnitDelay > state_with_error_0) {
      UnitDelay = state_with_error_0;
    } else if (UnitDelay < state_with_error) {
      UnitDelay = state_with_error;
    }

    /* Saturate: '<S2>/saturator' */
    skipper_lqi_for_export_B.saturator[i] = UnitDelay;
  }

  /* RateLimiter: '<S2>/Rate Limiter3' incorporates:
   *  Outport: '<Root>/Out13'
   */
  UnitDelay = skipper_lqi_for_export_B.saturator[0] -
    skipper_lqi_for_export_DW.PrevY;
  if (UnitDelay > skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.thrust =
      skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY;
  } else if (UnitDelay < skipper_lqi_for_export_P.RateLimiter3_FallingLim *
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
  UnitDelay = skipper_lqi_for_export_B.saturator[1] -
    skipper_lqi_for_export_DW.PrevY_k;
  if (UnitDelay > skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period) {
    skipper_lqi_for_export_Y.reaction_torque =
      skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_k;
  } else if (UnitDelay < skipper_lqi_for_export_P.RateLimiter2_FallingLim *
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
  UnitDelay = skipper_lqi_for_export_B.saturator[2] -
    skipper_lqi_for_export_DW.PrevY_b;
  if (UnitDelay > skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter1' */
    skipper_lqi_for_export_B.upper_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_b;
  } else if (UnitDelay < skipper_lqi_for_export_P.RateLimiter1_FallingLim *
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
  UnitDelay = skipper_lqi_for_export_B.saturator[3] -
    skipper_lqi_for_export_DW.PrevY_l;
  if (UnitDelay > skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period) {
    /* RateLimiter: '<S2>/Rate Limiter' */
    skipper_lqi_for_export_B.lower_gimbal_angle =
      skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_l;
  } else if (UnitDelay < skipper_lqi_for_export_P.RateLimiter_FallingLim *
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
  UnitDelay = skipper_lqi_for_export_Y.control[0];
  state_with_error = skipper_lqi_for_export_Y.control[1];
  state_with_error_0 = skipper_lqi_for_export_Y.control[2];
  fb_error_control = skipper_lqi_for_export_Y.control[3];
  for (i = 0; i < 12; i++) {
    /* Gain: '<Root>/passthrough_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     */
    passthrough_gain = skipper_lqi_for_export_P.D[i] * UnitDelay;
    passthrough_gain += skipper_lqi_for_export_P.D[i + 12] * state_with_error;
    passthrough_gain += skipper_lqi_for_export_P.D[i + 24] * state_with_error_0;
    passthrough_gain += skipper_lqi_for_export_P.D[i + 36] * fb_error_control;
    skipper_lqi_for_export_B.passthrough_gain[i] = passthrough_gain;

    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/output_gain'
     */
    skipper_lqi_for_export_B.output[i] = skipper_lqi_for_export_B.output_gain[i]
      + passthrough_gain;
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

  /* Outport: '<Root>/Out7' incorporates:
   *  Gain: '<Root>/Gain12'
   */
  skipper_lqi_for_export_Y.roll = skipper_lqi_for_export_P.Gain12_Gain *
    skipper_lqi_for_export_B.output[6];

  /* Outport: '<Root>/Out8' incorporates:
   *  Gain: '<Root>/Gain11'
   */
  skipper_lqi_for_export_Y.pitch = skipper_lqi_for_export_P.Gain11_Gain *
    skipper_lqi_for_export_B.output[7];

  /* Outport: '<Root>/Out9' incorporates:
   *  Gain: '<Root>/Gain10'
   */
  skipper_lqi_for_export_Y.yaw = skipper_lqi_for_export_P.Gain10_Gain *
    skipper_lqi_for_export_B.output[8];

  /* Outport: '<Root>/Out10' incorporates:
   *  Gain: '<Root>/Gain9'
   */
  skipper_lqi_for_export_Y.lateral_rate = skipper_lqi_for_export_P.Gain9_Gain *
    skipper_lqi_for_export_B.output[9];

  /* Outport: '<Root>/Out11' incorporates:
   *  Gain: '<Root>/Gain8'
   */
  skipper_lqi_for_export_Y.longitudinal_rate =
    skipper_lqi_for_export_P.Gain8_Gain * skipper_lqi_for_export_B.output[10];

  /* Outport: '<Root>/Out12' incorporates:
   *  Gain: '<Root>/Gain7'
   */
  skipper_lqi_for_export_Y.directional_rate =
    skipper_lqi_for_export_P.Gain7_Gain * skipper_lqi_for_export_B.output[11];

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
        UnitDelay = 0.0;
        for (i_1 = 0; i_1 < 12; i_1++) {
          UnitDelay += skipper_lqi_for_export_P.CovarianceZ_Value[12 * i_1 + i_0]
            * skipper_lqi_for_export_P.CovarianceZ_Value[12 * i_1 + i];
        }

        skipper_lqi_for_export_B.P[i_0 + 12 * i] = UnitDelay;
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
  tmp_2 = &skipper_lqi_for_export_P.A_Value[0];

  /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
  /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
  std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
              (real_T));
  for (i_0 = 0; i_0 < 12; i_0++) {
    passthrough_gain = 0.0;
    for (i = 0; i < 12; i++) {
      passthrough_gain += tmp_2[12 * i + i_0] * tmp_0[i];
    }

    /* Product: '<S23>/A[k]*xhat[k|k-1]' */
    skipper_lqi_for_export_B.Akxhatkk1[i_0] = passthrough_gain;
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
  UnitDelay = skipper_lqi_for_export_B.Reshapeu[0];
  state_with_error = skipper_lqi_for_export_B.Reshapeu[1];
  state_with_error_0 = skipper_lqi_for_export_B.Reshapeu[2];
  fb_error_control = skipper_lqi_for_export_B.Reshapeu[3];
  for (i_0 = 0; i_0 < 12; i_0++) {
    passthrough_gain = tmp_1[i_0] * UnitDelay;
    passthrough_gain += tmp_1[i_0 + 12] * state_with_error;
    passthrough_gain += tmp_1[i_0 + 24] * state_with_error_0;
    passthrough_gain += tmp_1[i_0 + 36] * fb_error_control;

    /* Product: '<S23>/B[k]*u[k]' */
    skipper_lqi_for_export_B.Bkuk[i_0] = passthrough_gain;
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
    tmp_2 = &skipper_lqi_for_export_P.C_Value[0];

    /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
                (real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      passthrough_gain = 0.0;
      for (i = 0; i < 12; i++) {
        passthrough_gain += tmp_2[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S54>/C[k]*xhat[k|k-1]' */
      skipper_lqi_for_export_B.Ckxhatkk1[i_0] = passthrough_gain;
    }

    /* Product: '<S54>/D[k]*u[k]' incorporates:
     *  Constant: '<S1>/D'
     */
    std::memcpy(&tmp_1[0], &skipper_lqi_for_export_P.D_Value[0], 48U * sizeof
                (real_T));
    for (i = 0; i < 12; i++) {
      passthrough_gain = tmp_1[i] * UnitDelay;
      passthrough_gain += tmp_1[i + 12] * state_with_error;
      passthrough_gain += tmp_1[i + 24] * state_with_error_0;
      passthrough_gain += tmp_1[i + 36] * fb_error_control;

      /* Product: '<S54>/D[k]*u[k]' */
      skipper_lqi_for_export_B.Dkuk[i] = passthrough_gain;

      /* Sum: '<S54>/Add1' */
      passthrough_gain += skipper_lqi_for_export_B.Ckxhatkk1[i];
      skipper_lqi_for_export_B.yhatkk1[i] = passthrough_gain;

      /* Sum: '<S54>/Sum' incorporates:
       *  Inport: '<Root>/imu_state_in'
       */
      skipper_lqi_for_export_B.Sum[i] = skipper_lqi_for_export_U.imu_state_in[i]
        - passthrough_gain;
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
    tmp_2 = &skipper_lqi_for_export_P.KalmanGainL_Value[0];

    /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.Sum[0], 12U * sizeof(real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      passthrough_gain = 0.0;
      for (i = 0; i < 12; i++) {
        passthrough_gain += tmp_2[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S54>/Product3' */
      skipper_lqi_for_export_B.Product3[i_0] = passthrough_gain;
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
  for (i = 0; i < 12; i++) {
    /* Sum: '<S23>/Add' */
    skipper_lqi_for_export_B.Add[i] = (skipper_lqi_for_export_B.Bkuk[i] +
      skipper_lqi_for_export_B.Akxhatkk1[i]) +
      skipper_lqi_for_export_B.Product3[i];
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
    tmp_2 = &skipper_lqi_for_export_P.C_Value[0];

    /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.MemoryX[0], 12U * sizeof
                (real_T));
    for (i = 0; i < 12; i++) {
      UnitDelay = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        UnitDelay += tmp_2[12 * i_0 + i] * tmp_0[i_0];
      }

      /* Product: '<S56>/Product' */
      skipper_lqi_for_export_B.Product[i] = UnitDelay;

      /* Sum: '<S56>/Add1' incorporates:
       *  Inport: '<Root>/imu_state_in'
       */
      skipper_lqi_for_export_B.Add1[i] = skipper_lqi_for_export_U.imu_state_in[i]
        - UnitDelay;
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
    tmp_2 = &skipper_lqi_for_export_P.KalmanGainM_Value[0];

    /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
    std::memcpy(&tmp_0[0], &skipper_lqi_for_export_B.Add1[0], 12U * sizeof
                (real_T));
    for (i_0 = 0; i_0 < 12; i_0++) {
      passthrough_gain = 0.0;
      for (i = 0; i < 12; i++) {
        passthrough_gain += tmp_2[12 * i + i_0] * tmp_0[i];
      }

      /* Product: '<S56>/Product2' */
      skipper_lqi_for_export_B.Product2[i_0] = passthrough_gain;
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
  UnitDelay = skipper_lqi_for_export_Y.control[0];
  state_with_error = skipper_lqi_for_export_Y.control[1];
  state_with_error_0 = skipper_lqi_for_export_Y.control[2];
  fb_error_control = skipper_lqi_for_export_Y.control[3];
  for (i = 0; i < 12; i++) {
    /* Sum: '<S30>/Add' */
    passthrough_gain = skipper_lqi_for_export_B.Product2[i] +
      skipper_lqi_for_export_B.MemoryX[i];
    skipper_lqi_for_export_B.Add_j[i] = passthrough_gain;

    /* Gain: '<Root>/input_gain' incorporates:
     *  Outport: '<Root>/control_commands_out'
     */
    input_gain = skipper_lqi_for_export_P.B[i] * UnitDelay;
    input_gain += skipper_lqi_for_export_P.B[i + 12] * state_with_error;
    input_gain += skipper_lqi_for_export_P.B[i + 24] * state_with_error_0;
    input_gain += skipper_lqi_for_export_P.B[i + 36] * fb_error_control;
    skipper_lqi_for_export_B.input_gain[i] = input_gain;

    /* Update for UnitDelay: '<Root>/Unit Delay' */
    skipper_lqi_for_export_DW.UnitDelay_DSTATE[i] = passthrough_gain;

    /* Update for UnitDelay: '<Root>/Unit Delay1' */
    skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i] = input_gain;
  }

  /* Update for DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
   *  Sum: '<Root>/state_derivative_sum'
   */
  for (i = 0; i < 15; i++) {
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
}

/* Model initialize function */
void skipper_lqi_for_export::initialize()
{
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
