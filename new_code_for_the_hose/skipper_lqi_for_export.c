/*
 * skipper_lqi_for_export.c
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi_for_export".
 *
 * Model version              : 3.0
 * Simulink Coder version : 24.1 (R2024a) 19-Nov-2023
 * C source code generated on : Mon Oct 13 08:13:31 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "skipper_lqi_for_export.h"
#include "rtwtypes.h"
#include <string.h>
#include <emmintrin.h>
#include "skipper_lqi_for_export_private.h"
#include "rt_nonfinite.h"
#define skipper_lqi_for_export_period  (0.05)

/* Block signals (default storage) */
B_skipper_lqi_for_export_T skipper_lqi_for_export_B;

/* Block states (default storage) */
DW_skipper_lqi_for_export_T skipper_lqi_for_export_DW;

/* External inputs (root inport signals with default storage) */
ExtU_skipper_lqi_for_export_T skipper_lqi_for_export_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_skipper_lqi_for_export_T skipper_lqi_for_export_Y;

/* Real-time model */
static RT_MODEL_skipper_lqi_for_expo_T skipper_lqi_for_export_M_;
RT_MODEL_skipper_lqi_for_expo_T *const skipper_lqi_for_export_M =
  &skipper_lqi_for_export_M_;

/* Model step function */
void skipper_lqi_for_export_step(void)
{
  __m128d tmp_7;
  __m128d tmp_8;
  __m128d tmp_9;
  real_T tmp_5[48];
  real_T tmp_0[36];
  real_T state_with_error[15];
  real_T state_with_error_tmp[15];
  real_T tmp_2[15];
  real_T tmp_3[15];
  real_T tmp_4[15];
  real_T rtb_Bkuk[12];
  real_T tmp[12];
  real_T tmp_6[12];
  real_T rtb_saturator[4];
  real_T tmp_1[3];
  real_T tmp_a[2];
  real_T rtb_Bkuk_0;
  real_T rtb_counter_rotating;
  real_T rtb_lower_gimbal_angle;
  real_T rtb_thrust;
  real_T rtb_upper_gimbal_angle;
  int32_T i;
  int32_T i_0;
  for (i = 0; i <= 10; i += 2) {
    /* Sum: '<Root>/feedback_sum' incorporates:
     *  Constant: '<Root>/perturbation'
     *  UnitDelay: '<Root>/Unit Delay'
     */
    tmp_9 = _mm_loadu_pd(&skipper_lqi_for_export_DW.UnitDelay_DSTATE[i]);
    _mm_storeu_pd(&rtb_Bkuk[i], _mm_add_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.perturbation_Value[i]), tmp_9));

    /* Constant: '<Root>/initial_state' incorporates:
     *  Sum: '<Root>/feedback_sum'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_B.initial_state[i], _mm_loadu_pd
                  (&skipper_lqi_for_export_P.initial_state_Value[i]));
  }

  /* Constant: '<Root>/initial_error ' */
  skipper_lqi_for_export_B.initial_error[0] =
    skipper_lqi_for_export_P.initial_error_Value[0];
  skipper_lqi_for_export_B.initial_error[1] =
    skipper_lqi_for_export_P.initial_error_Value[1];
  skipper_lqi_for_export_B.initial_error[2] =
    skipper_lqi_for_export_P.initial_error_Value[2];

  /* Gain: '<Root>/state_gain' */
  for (i_0 = 0; i_0 < 12; i_0++) {
    rtb_Bkuk_0 = 0.0;
    for (i = 0; i < 12; i++) {
      rtb_Bkuk_0 += skipper_lqi_for_export_P.A[12 * i + i_0] * rtb_Bkuk[i];
    }

    tmp[i_0] = rtb_Bkuk_0;
  }

  /* End of Gain: '<Root>/state_gain' */

  /* Gain: '<Root>/extraction_gain' */
  for (i_0 = 0; i_0 <= 34; i_0 += 2) {
    _mm_storeu_pd(&tmp_0[i_0], _mm_mul_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.E[i_0]), _mm_set1_pd(-1.0)));
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    rtb_Bkuk_0 = 0.0;
    for (i = 0; i < 12; i++) {
      rtb_Bkuk_0 += tmp_0[3 * i + i_0] * rtb_Bkuk[i];
    }

    tmp_1[i_0] = rtb_Bkuk_0;
  }

  /* End of Gain: '<Root>/extraction_gain' */

  /* Sum: '<Root>/state_derivative_sum' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Constant: '<Root>/gravity_const'
   *  Inport: '<Root>/refeference_in'
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  tmp_2[12] = skipper_lqi_for_export_U.reference[0];
  tmp_2[13] = skipper_lqi_for_export_U.reference[1];
  tmp_2[14] = skipper_lqi_for_export_U.reference[2];
  tmp_3[12] = tmp_1[0];
  tmp_3[13] = tmp_1[1];
  tmp_3[14] = tmp_1[2];
  memcpy(&tmp_2[0], &skipper_lqi_for_export_P.d[0], 12U * sizeof(real_T));
  memcpy(&tmp_3[0], &tmp[0], 12U * sizeof(real_T));
  memcpy(&tmp_4[0], &skipper_lqi_for_export_DW.UnitDelay1_DSTATE[0], 12U *
         sizeof(real_T));
  tmp_4[12] = skipper_lqi_for_export_P.dummy_zeros_Value[0];
  tmp_4[13] = skipper_lqi_for_export_P.dummy_zeros_Value[1];
  tmp_4[14] = skipper_lqi_for_export_P.dummy_zeros_Value[2];
  for (i_0 = 0; i_0 <= 12; i_0 += 2) {
    /* Sum: '<Root>/state_derivative_sum' */
    tmp_9 = _mm_loadu_pd(&tmp_2[i_0]);
    tmp_7 = _mm_loadu_pd(&tmp_3[i_0]);
    tmp_8 = _mm_loadu_pd(&tmp_4[i_0]);

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    tmp_9 = _mm_mul_pd(_mm_add_pd(_mm_add_pd(tmp_9, tmp_7), tmp_8), _mm_set1_pd
                       (skipper_lqi_for_export_P.DiscreteIntegrator_gainval));
    _mm_storeu_pd(&state_with_error_tmp[i_0], tmp_9);
    tmp_7 = _mm_loadu_pd
      (&skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i_0]);

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    _mm_storeu_pd(&state_with_error[i_0], _mm_add_pd(tmp_7, tmp_9));
  }

  for (i_0 = 14; i_0 < 15; i_0++) {
    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    rtb_lower_gimbal_angle = ((tmp_2[i_0] + tmp_3[i_0]) + tmp_4[i_0]) *
      skipper_lqi_for_export_P.DiscreteIntegrator_gainval;
    state_with_error_tmp[i_0] = rtb_lower_gimbal_angle;

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' */
    state_with_error[i_0] =
      skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i_0] +
      rtb_lower_gimbal_angle;
  }

  /* Gain: '<Root>/lqi_gain' */
  for (i_0 = 0; i_0 <= 10; i_0 += 2) {
    _mm_storeu_pd(&tmp[i_0], _mm_mul_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.K2[i_0]), _mm_set1_pd(-1.0)));
  }

  /* Gain: '<Root>/lqr_gain' */
  for (i_0 = 0; i_0 <= 46; i_0 += 2) {
    _mm_storeu_pd(&tmp_5[i_0], _mm_mul_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.K1[i_0]), _mm_set1_pd(-1.0)));
  }

  for (i_0 = 0; i_0 < 4; i_0++) {
    /* Gain: '<Root>/lqr_gain' */
    rtb_Bkuk_0 = 0.0;
    for (i = 0; i < 12; i++) {
      rtb_Bkuk_0 += tmp_5[(i << 2) + i_0] * state_with_error[i];
    }

    /* Saturate: '<S2>/saturator' incorporates:
     *  Constant: '<Root>/ff_control'
     *  Gain: '<Root>/lqi_gain'
     *  Gain: '<Root>/lqr_gain'
     *  Sum: '<Root>/control_sum'
     */
    rtb_lower_gimbal_angle = (((tmp[i_0 + 4] * state_with_error[13] + tmp[i_0] *
      state_with_error[12]) + tmp[i_0 + 8] * state_with_error[14]) + rtb_Bkuk_0)
      + skipper_lqi_for_export_P.Uff[i_0];
    rtb_thrust = skipper_lqi_for_export_P.saturator_LowerSat[i_0];
    rtb_counter_rotating = skipper_lqi_for_export_P.saturator_UpperSat[i_0];
    if (rtb_lower_gimbal_angle > rtb_counter_rotating) {
      rtb_saturator[i_0] = rtb_counter_rotating;
    } else if (rtb_lower_gimbal_angle < rtb_thrust) {
      rtb_saturator[i_0] = rtb_thrust;
    } else {
      rtb_saturator[i_0] = rtb_lower_gimbal_angle;
    }

    /* End of Saturate: '<S2>/saturator' */
  }

  /* RateLimiter: '<S2>/Rate Limiter3' */
  rtb_lower_gimbal_angle = rtb_saturator[0] - skipper_lqi_for_export_DW.PrevY;
  if (rtb_lower_gimbal_angle > skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period) {
    rtb_thrust = skipper_lqi_for_export_P.RateLimiter3_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY;
  } else if (rtb_lower_gimbal_angle <
             skipper_lqi_for_export_P.RateLimiter3_FallingLim *
             skipper_lqi_for_export_period) {
    rtb_thrust = skipper_lqi_for_export_P.RateLimiter3_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY;
  } else {
    rtb_thrust = rtb_saturator[0];
  }

  skipper_lqi_for_export_DW.PrevY = rtb_thrust;

  /* End of RateLimiter: '<S2>/Rate Limiter3' */

  /* RateLimiter: '<S2>/Rate Limiter2' */
  rtb_lower_gimbal_angle = rtb_saturator[1] - skipper_lqi_for_export_DW.PrevY_k;
  if (rtb_lower_gimbal_angle > skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period) {
    rtb_counter_rotating = skipper_lqi_for_export_P.RateLimiter2_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_k;
  } else if (rtb_lower_gimbal_angle <
             skipper_lqi_for_export_P.RateLimiter2_FallingLim *
             skipper_lqi_for_export_period) {
    rtb_counter_rotating = skipper_lqi_for_export_P.RateLimiter2_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_k;
  } else {
    rtb_counter_rotating = rtb_saturator[1];
  }

  skipper_lqi_for_export_DW.PrevY_k = rtb_counter_rotating;

  /* End of RateLimiter: '<S2>/Rate Limiter2' */

  /* RateLimiter: '<S2>/Rate Limiter1' */
  rtb_lower_gimbal_angle = rtb_saturator[2] - skipper_lqi_for_export_DW.PrevY_b;
  if (rtb_lower_gimbal_angle > skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period) {
    rtb_upper_gimbal_angle = skipper_lqi_for_export_P.RateLimiter1_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_b;
  } else if (rtb_lower_gimbal_angle <
             skipper_lqi_for_export_P.RateLimiter1_FallingLim *
             skipper_lqi_for_export_period) {
    rtb_upper_gimbal_angle = skipper_lqi_for_export_P.RateLimiter1_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_b;
  } else {
    rtb_upper_gimbal_angle = rtb_saturator[2];
  }

  skipper_lqi_for_export_DW.PrevY_b = rtb_upper_gimbal_angle;

  /* End of RateLimiter: '<S2>/Rate Limiter1' */

  /* RateLimiter: '<S2>/Rate Limiter' */
  rtb_lower_gimbal_angle = rtb_saturator[3] - skipper_lqi_for_export_DW.PrevY_l;
  if (rtb_lower_gimbal_angle > skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period) {
    rtb_lower_gimbal_angle = skipper_lqi_for_export_P.RateLimiter_RisingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_l;
  } else if (rtb_lower_gimbal_angle <
             skipper_lqi_for_export_P.RateLimiter_FallingLim *
             skipper_lqi_for_export_period) {
    rtb_lower_gimbal_angle = skipper_lqi_for_export_P.RateLimiter_FallingLim *
      skipper_lqi_for_export_period + skipper_lqi_for_export_DW.PrevY_l;
  } else {
    rtb_lower_gimbal_angle = rtb_saturator[3];
  }

  skipper_lqi_for_export_DW.PrevY_l = rtb_lower_gimbal_angle;

  /* End of RateLimiter: '<S2>/Rate Limiter' */

  /* Outport: '<Root>/control_commands_out' incorporates:
   *  SignalConversion generated from: '<Root>/input_gain'
   */
  skipper_lqi_for_export_Y.control[0] = rtb_thrust;
  skipper_lqi_for_export_Y.control[1] = rtb_counter_rotating;
  skipper_lqi_for_export_Y.control[2] = rtb_upper_gimbal_angle;
  skipper_lqi_for_export_Y.control[3] = rtb_lower_gimbal_angle;

  /* Outport: '<Root>/Out13' */
  skipper_lqi_for_export_Y.thrust = rtb_thrust;

  /* Outport: '<Root>/Out14' */
  skipper_lqi_for_export_Y.reaction_torque = rtb_counter_rotating;

  /* Outport: '<Root>/Out15' incorporates:
   *  Gain: '<Root>/Gain5'
   */
  skipper_lqi_for_export_Y.upper_gimbal_angle =
    skipper_lqi_for_export_P.Gain5_Gain * rtb_upper_gimbal_angle;

  /* Outport: '<Root>/Out16' incorporates:
   *  Gain: '<Root>/Gain6'
   */
  skipper_lqi_for_export_Y.lower_gimbal_angle =
    skipper_lqi_for_export_P.Gain6_Gain * rtb_lower_gimbal_angle;

  /* Outport: '<Root>/Out17' */
  skipper_lqi_for_export_Y.altitude_error = state_with_error[12];

  /* Outport: '<Root>/Out18' */
  skipper_lqi_for_export_Y.crossrange_error = state_with_error[13];

  /* Outport: '<Root>/Out19' */
  skipper_lqi_for_export_Y.downrange_error = state_with_error[14];

  /* Delay: '<S1>/MemoryX' incorporates:
   *  Constant: '<S1>/X0'
   */
  if (skipper_lqi_for_export_DW.icLoad) {
    memcpy(&skipper_lqi_for_export_DW.MemoryX_DSTATE[0],
           &skipper_lqi_for_export_P.X0_Value[0], 12U * sizeof(real_T));
  }

  for (i = 0; i < 12; i++) {
    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/output_gain'
     *  Gain: '<Root>/passthrough_gain'
     *  SignalConversion generated from: '<Root>/input_gain'
     *  Sum: '<S56>/Add1'
     */
    rtb_Bkuk_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtb_Bkuk_0 += skipper_lqi_for_export_P.C[12 * i_0 + i] *
        state_with_error[i_0];
    }

    rtb_Bkuk[i] = (((skipper_lqi_for_export_P.D[i + 12] * rtb_counter_rotating +
                     skipper_lqi_for_export_P.D[i] * rtb_thrust) +
                    skipper_lqi_for_export_P.D[i + 24] * rtb_upper_gimbal_angle)
                   + skipper_lqi_for_export_P.D[i + 36] * rtb_lower_gimbal_angle)
      + rtb_Bkuk_0;

    /* End of Sum: '<Root>/Sum2' */
  }

  /* Outport: '<Root>/Out1' */
  skipper_lqi_for_export_Y.altitude = rtb_Bkuk[0];

  /* Outport: '<Root>/Out2' */
  skipper_lqi_for_export_Y.crossrange = rtb_Bkuk[1];

  /* Outport: '<Root>/Out3' */
  skipper_lqi_for_export_Y.downrange = rtb_Bkuk[2];

  /* Outport: '<Root>/Out4' */
  skipper_lqi_for_export_Y.lateral_vel = rtb_Bkuk[3];

  /* Outport: '<Root>/Out5' */
  skipper_lqi_for_export_Y.longitudinal_vel = rtb_Bkuk[4];

  /* Outport: '<Root>/Out6' */
  skipper_lqi_for_export_Y.directional_vel = rtb_Bkuk[5];

  /* Gain: '<Root>/Gain11' incorporates:
   *  Gain: '<Root>/Gain12'
   */
  tmp_9 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain11_Gain,
    skipper_lqi_for_export_P.Gain12_Gain), _mm_loadu_pd(&rtb_Bkuk[6]));
  _mm_storeu_pd(&tmp_a[0], tmp_9);

  /* Outport: '<Root>/Out7' */
  skipper_lqi_for_export_Y.roll = tmp_a[0];

  /* Outport: '<Root>/Out8' */
  skipper_lqi_for_export_Y.pitch = tmp_a[1];

  /* Gain: '<Root>/Gain9' incorporates:
   *  Gain: '<Root>/Gain10'
   */
  tmp_9 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain9_Gain,
    skipper_lqi_for_export_P.Gain10_Gain), _mm_loadu_pd(&rtb_Bkuk[8]));
  _mm_storeu_pd(&tmp_a[0], tmp_9);

  /* Outport: '<Root>/Out9' */
  skipper_lqi_for_export_Y.yaw = tmp_a[0];

  /* Outport: '<Root>/Out10' */
  skipper_lqi_for_export_Y.lateral_rate = tmp_a[1];

  /* Gain: '<Root>/Gain7' incorporates:
   *  Gain: '<Root>/Gain8'
   */
  tmp_9 = _mm_mul_pd(_mm_set_pd(skipper_lqi_for_export_P.Gain7_Gain,
    skipper_lqi_for_export_P.Gain8_Gain), _mm_loadu_pd(&rtb_Bkuk[10]));
  _mm_storeu_pd(&tmp_a[0], tmp_9);

  /* Outport: '<Root>/Out11' */
  skipper_lqi_for_export_Y.longitudinal_rate = tmp_a[0];

  /* Outport: '<Root>/Out12' */
  skipper_lqi_for_export_Y.directional_rate = tmp_a[1];

  /* Outputs for Enabled SubSystem: '<S30>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S56>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S23>/MeasurementUpdate' incorporates:
   *  EnablePort: '<S54>/Enable'
   */
  /* Constant: '<S1>/Enable' */
  if (skipper_lqi_for_export_P.Enable_Value) {
    skipper_lqi_for_export_DW.MeasurementUpdate_MODE = true;
    for (i_0 = 0; i_0 < 12; i_0++) {
      /* Product: '<S54>/C[k]*xhat[k|k-1]' incorporates:
       *  Constant: '<S1>/C'
       */
      rtb_Bkuk_0 = 0.0;
      for (i = 0; i < 12; i++) {
        /* Product: '<S56>/Product' incorporates:
         *  Constant: '<S1>/C'
         *  Delay: '<S1>/MemoryX'
         */
        rtb_Bkuk_0 += skipper_lqi_for_export_P.C_Value[12 * i + i_0] *
          skipper_lqi_for_export_DW.MemoryX_DSTATE[i];
      }

      rtb_Bkuk[i_0] = rtb_Bkuk_0;

      /* Sum: '<S54>/Sum' incorporates:
       *  Constant: '<S1>/D'
       *  Inport: '<Root>/imu_state_in'
       *  Product: '<S54>/C[k]*xhat[k|k-1]'
       *  Product: '<S54>/D[k]*u[k]'
       *  SignalConversion generated from: '<Root>/input_gain'
       *  Sum: '<S54>/Add1'
       */
      tmp[i_0] = skipper_lqi_for_export_U.imu_state_in[i_0] -
        ((((skipper_lqi_for_export_P.D_Value[i_0 + 12] * rtb_counter_rotating +
            skipper_lqi_for_export_P.D_Value[i_0] * rtb_thrust) +
           skipper_lqi_for_export_P.D_Value[i_0 + 24] * rtb_upper_gimbal_angle)
          + skipper_lqi_for_export_P.D_Value[i_0 + 36] * rtb_lower_gimbal_angle)
         + rtb_Bkuk_0);
    }

    skipper_lqi_for_export_DW.EnabledSubsystem_MODE = true;
    for (i_0 = 0; i_0 < 12; i_0++) {
      /* Product: '<S54>/Product3' incorporates:
       *  Constant: '<S3>/KalmanGainL'
       */
      rtb_Bkuk_0 = 0.0;
      for (i = 0; i < 12; i++) {
        rtb_Bkuk_0 += skipper_lqi_for_export_P.KalmanGainL_Value[12 * i + i_0] *
          tmp[i];
      }

      skipper_lqi_for_export_B.Product3[i_0] = rtb_Bkuk_0;

      /* End of Product: '<S54>/Product3' */

      /* Sum: '<S56>/Add1' incorporates:
       *  Inport: '<Root>/imu_state_in'
       *  Product: '<S56>/Product'
       */
      tmp_6[i_0] = skipper_lqi_for_export_U.imu_state_in[i_0] - rtb_Bkuk[i_0];
    }

    for (i_0 = 0; i_0 < 12; i_0++) {
      /* Product: '<S56>/Product2' incorporates:
       *  Constant: '<S3>/KalmanGainM'
       */
      rtb_Bkuk_0 = 0.0;
      for (i = 0; i < 12; i++) {
        rtb_Bkuk_0 += skipper_lqi_for_export_P.KalmanGainM_Value[12 * i + i_0] *
          tmp_6[i];
      }

      skipper_lqi_for_export_B.Product2[i_0] = rtb_Bkuk_0;

      /* End of Product: '<S56>/Product2' */
    }
  } else {
    if (skipper_lqi_for_export_DW.MeasurementUpdate_MODE) {
      for (i = 0; i < 12; i++) {
        /* Disable for Product: '<S54>/Product3' incorporates:
         *  Outport: '<S54>/L*(y[k]-yhat[k|k-1])'
         */
        skipper_lqi_for_export_B.Product3[i] =
          skipper_lqi_for_export_P.Lykyhatkk1_Y0;
      }

      skipper_lqi_for_export_DW.MeasurementUpdate_MODE = false;
    }

    if (skipper_lqi_for_export_DW.EnabledSubsystem_MODE) {
      for (i = 0; i < 12; i++) {
        /* Disable for Product: '<S56>/Product2' incorporates:
         *  Outport: '<S56>/deltax'
         */
        skipper_lqi_for_export_B.Product2[i] =
          skipper_lqi_for_export_P.deltax_Y0;
      }

      skipper_lqi_for_export_DW.EnabledSubsystem_MODE = false;
    }
  }

  /* End of Constant: '<S1>/Enable' */
  /* End of Outputs for SubSystem: '<S23>/MeasurementUpdate' */
  /* End of Outputs for SubSystem: '<S30>/Enabled Subsystem' */
  for (i = 0; i <= 10; i += 2) {
    /* Sum: '<S30>/Add' incorporates:
     *  Delay: '<S1>/MemoryX'
     *  UnitDelay: '<Root>/Unit Delay'
     */
    tmp_9 = _mm_loadu_pd(&skipper_lqi_for_export_B.Product2[i]);
    tmp_7 = _mm_loadu_pd(&skipper_lqi_for_export_DW.MemoryX_DSTATE[i]);

    /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
     *  Sum: '<S30>/Add'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_DW.UnitDelay_DSTATE[i], _mm_add_pd
                  (tmp_9, tmp_7));

    /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
     *  Gain: '<Root>/input_gain'
     *  SignalConversion generated from: '<Root>/input_gain'
     *  UnitDelay: '<Root>/Unit Delay'
     */
    _mm_storeu_pd(&skipper_lqi_for_export_DW.UnitDelay1_DSTATE[i], _mm_add_pd
                  (_mm_add_pd(_mm_add_pd(_mm_mul_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.B[i + 12]), _mm_set1_pd(rtb_counter_rotating)),
      _mm_mul_pd(_mm_loadu_pd(&skipper_lqi_for_export_P.B[i]), _mm_set1_pd
                 (rtb_thrust))), _mm_mul_pd(_mm_loadu_pd
      (&skipper_lqi_for_export_P.B[i + 24]), _mm_set1_pd(rtb_upper_gimbal_angle))),
                   _mm_mul_pd(_mm_loadu_pd(&skipper_lqi_for_export_P.B[i + 36]),
      _mm_set1_pd(rtb_lower_gimbal_angle))));
  }

  /* Update for DiscreteIntegrator: '<Root>/Discrete Integrator' */
  for (i_0 = 0; i_0 <= 12; i_0 += 2) {
    tmp_9 = _mm_loadu_pd(&state_with_error[i_0]);
    tmp_7 = _mm_loadu_pd(&state_with_error_tmp[i_0]);
    _mm_storeu_pd(&skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i_0],
                  _mm_add_pd(tmp_9, tmp_7));
  }

  for (i_0 = 14; i_0 < 15; i_0++) {
    skipper_lqi_for_export_DW.DiscreteIntegrator_DSTATE[i_0] =
      state_with_error[i_0] + state_with_error_tmp[i_0];
  }

  /* Update for Delay: '<S1>/MemoryX' */
  skipper_lqi_for_export_DW.icLoad = false;
  for (i_0 = 0; i_0 < 12; i_0++) {
    /* Product: '<S23>/B[k]*u[k]' incorporates:
     *  Constant: '<S1>/B'
     *  SignalConversion generated from: '<Root>/input_gain'
     */
    tmp[i_0] = ((skipper_lqi_for_export_P.B_Value[i_0 + 12] *
                 rtb_counter_rotating + skipper_lqi_for_export_P.B_Value[i_0] *
                 rtb_thrust) + skipper_lqi_for_export_P.B_Value[i_0 + 24] *
                rtb_upper_gimbal_angle) + skipper_lqi_for_export_P.B_Value[i_0 +
      36] * rtb_lower_gimbal_angle;

    /* Product: '<S23>/A[k]*xhat[k|k-1]' incorporates:
     *  Constant: '<S1>/A'
     *  Delay: '<S1>/MemoryX'
     */
    rtb_Bkuk_0 = 0.0;
    for (i = 0; i < 12; i++) {
      rtb_Bkuk_0 += skipper_lqi_for_export_P.A_Value[12 * i + i_0] *
        skipper_lqi_for_export_DW.MemoryX_DSTATE[i];
    }

    tmp_6[i_0] = rtb_Bkuk_0;

    /* End of Product: '<S23>/A[k]*xhat[k|k-1]' */
  }

  /* Update for Delay: '<S1>/MemoryX' incorporates:
   *  Sum: '<S23>/Add'
   */
  for (i_0 = 0; i_0 <= 10; i_0 += 2) {
    /* Sum: '<S23>/Add' */
    tmp_9 = _mm_loadu_pd(&tmp[i_0]);
    tmp_7 = _mm_loadu_pd(&tmp_6[i_0]);
    tmp_8 = _mm_loadu_pd(&skipper_lqi_for_export_B.Product3[i_0]);
    _mm_storeu_pd(&skipper_lqi_for_export_DW.MemoryX_DSTATE[i_0], _mm_add_pd
                  (_mm_add_pd(tmp_9, tmp_7), tmp_8));
  }

  /* Matfile logging */
  rt_UpdateTXYLogVars(skipper_lqi_for_export_M->rtwLogInfo,
                      (&skipper_lqi_for_export_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.05s, 0.0s] */
    if ((rtmGetTFinal(skipper_lqi_for_export_M)!=-1) &&
        !((rtmGetTFinal(skipper_lqi_for_export_M)-
           skipper_lqi_for_export_M->Timing.taskTime0) >
          skipper_lqi_for_export_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(skipper_lqi_for_export_M, "Simulation finished");
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
  if (!(++skipper_lqi_for_export_M->Timing.clockTick0)) {
    ++skipper_lqi_for_export_M->Timing.clockTickH0;
  }

  skipper_lqi_for_export_M->Timing.taskTime0 =
    skipper_lqi_for_export_M->Timing.clockTick0 *
    skipper_lqi_for_export_M->Timing.stepSize0 +
    skipper_lqi_for_export_M->Timing.clockTickH0 *
    skipper_lqi_for_export_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void skipper_lqi_for_export_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  skipper_lqi_for_export_P.RateLimiter3_RisingLim = rtInf;
  skipper_lqi_for_export_P.RateLimiter3_FallingLim = rtMinusInf;
  skipper_lqi_for_export_P.RateLimiter2_RisingLim = rtInf;
  skipper_lqi_for_export_P.RateLimiter2_FallingLim = rtMinusInf;
  skipper_lqi_for_export_P.RateLimiter1_RisingLim = rtInf;
  skipper_lqi_for_export_P.RateLimiter1_FallingLim = rtMinusInf;
  skipper_lqi_for_export_P.RateLimiter_RisingLim = rtInf;
  skipper_lqi_for_export_P.RateLimiter_FallingLim = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)skipper_lqi_for_export_M, 0,
                sizeof(RT_MODEL_skipper_lqi_for_expo_T));
  rtmSetTFinal(skipper_lqi_for_export_M, 25.0);
  skipper_lqi_for_export_M->Timing.stepSize0 = 0.05;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    skipper_lqi_for_export_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(skipper_lqi_for_export_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(skipper_lqi_for_export_M->rtwLogInfo, (NULL));
    rtliSetLogT(skipper_lqi_for_export_M->rtwLogInfo, "tout");
    rtliSetLogX(skipper_lqi_for_export_M->rtwLogInfo, "");
    rtliSetLogXFinal(skipper_lqi_for_export_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(skipper_lqi_for_export_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(skipper_lqi_for_export_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(skipper_lqi_for_export_M->rtwLogInfo, 0);
    rtliSetLogDecimation(skipper_lqi_for_export_M->rtwLogInfo, 1);
    rtliSetLogY(skipper_lqi_for_export_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(skipper_lqi_for_export_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(skipper_lqi_for_export_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &skipper_lqi_for_export_B), 0,
                sizeof(B_skipper_lqi_for_export_T));

  /* states (dwork) */
  (void) memset((void *)&skipper_lqi_for_export_DW, 0,
                sizeof(DW_skipper_lqi_for_export_T));

  /* external inputs */
  (void)memset(&skipper_lqi_for_export_U, 0, sizeof
               (ExtU_skipper_lqi_for_export_T));

  /* external outputs */
  (void)memset(&skipper_lqi_for_export_Y, 0, sizeof
               (ExtY_skipper_lqi_for_export_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(skipper_lqi_for_export_M->rtwLogInfo, 0.0,
    rtmGetTFinal(skipper_lqi_for_export_M),
    skipper_lqi_for_export_M->Timing.stepSize0, (&rtmGetErrorStatus
    (skipper_lqi_for_export_M)));

  /* Start for Constant: '<Root>/initial_state' */
  memcpy(&skipper_lqi_for_export_B.initial_state[0],
         &skipper_lqi_for_export_P.initial_state_Value[0], 12U * sizeof(real_T));

  /* Start for Constant: '<Root>/initial_error ' */
  skipper_lqi_for_export_B.initial_error[0] =
    skipper_lqi_for_export_P.initial_error_Value[0];
  skipper_lqi_for_export_B.initial_error[1] =
    skipper_lqi_for_export_P.initial_error_Value[1];
  skipper_lqi_for_export_B.initial_error[2] =
    skipper_lqi_for_export_P.initial_error_Value[2];

  {
    int32_T i;

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
    for (i = 0; i < 12; i++) {
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
}

/* Model terminate function */
void skipper_lqi_for_export_terminate(void)
{
  /* (no terminate code required) */
}
