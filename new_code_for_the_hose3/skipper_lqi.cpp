/*
 * skipper_lqi.cpp
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi".
 *
 * Model version              : 2.6
 * Simulink Coder version : 24.1 (R2024a) 19-Nov-2023
 * C++ source code generated on : Mon Oct 13 16:56:40 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM 7
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "skipper_lqi.h"
#include "rtwtypes.h"
#include <cstring>
#include <cmath>
#include "skipper_lqi_private.h"
#include "cmath"

const real_T skipper_lqi_period{ 0.05 };

static void rate_scheduler(RT_MODEL_skipper_lqi_T *const skipper_lqi_M);

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(RT_MODEL_skipper_lqi_T *const skipper_lqi_M)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (skipper_lqi_M->Timing.TaskCounters.TID[1])++;
  if ((skipper_lqi_M->Timing.TaskCounters.TID[1]) > 1) {/* Sample time: [0.1s, 0.0s] */
    skipper_lqi_M->Timing.TaskCounters.TID[1] = 0;
  }
}

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = std::sqrt(-2.0 * std::log(si) / si) * sr;
  return y;
}

/* Model step function */
void skipper_lqi::step()
{
  real_T tmp_4[48];
  real_T tmp[36];
  real_T state_with_error[15];
  real_T state_with_error_tmp[15];
  real_T tmp_1[15];
  real_T tmp_2[15];
  real_T tmp_3[15];
  real_T rtb_Akxhatkk1[12];
  real_T rtb_Akxhatkk1_0[12];
  real_T rtb_Akxhatkk1_1[12];
  real_T x[12];
  real_T rtb_saturator[4];
  real_T tmp_0[3];
  real_T rtb_counter_rotating;
  real_T rtb_lower_gimbal_angle;
  real_T rtb_thrust;
  real_T rtb_upper_gimbal_angle;
  real_T x_0;
  int32_T i;
  int32_T i_0;
  for (i = 0; i < 12; i++) {
    /* Sum: '<Root>/feedback_sum' incorporates:
     *  Constant: '<Root>/perturbation'
     *  UnitDelay: '<Root>/Unit Delay'
     */
    rtb_Akxhatkk1[i] = skipper_lqi_P.perturbation_Value[i] +
      skipper_lqi_DW.UnitDelay_DSTATE[i];

    /* Constant: '<Root>/initial_state' */
    skipper_lqi_B.initial_state[i] = skipper_lqi_P.initial_state_Value[i];
  }

  /* Constant: '<Root>/initial_error' */
  skipper_lqi_B.initial_error[0] = skipper_lqi_P.initial_error_Value[0];
  skipper_lqi_B.initial_error[1] = skipper_lqi_P.initial_error_Value[1];
  skipper_lqi_B.initial_error[2] = skipper_lqi_P.initial_error_Value[2];

  /* Gain: '<Root>/extraction_gain' */
  for (i = 0; i < 36; i++) {
    tmp[i] = -skipper_lqi_P.E[i];
  }

  /* Gain: '<Root>/state_gain' */
  for (i = 0; i < 12; i++) {
    x_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      x_0 += skipper_lqi_P.A[12 * i_0 + i] * rtb_Akxhatkk1[i_0];
    }

    x[i] = x_0;
  }

  /* End of Gain: '<Root>/state_gain' */

  /* Gain: '<Root>/extraction_gain' */
  for (i = 0; i < 3; i++) {
    x_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      x_0 += tmp[3 * i_0 + i] * rtb_Akxhatkk1[i_0];
    }

    tmp_0[i] = x_0;
  }

  /* Sum: '<Root>/state_derivative_sum' incorporates:
   *  Constant: '<Root>/dummy_zeros'
   *  Constant: '<Root>/gravity_const'
   *  Constant: '<Root>/reference_const'
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  tmp_1[12] = skipper_lqi_P.reference_const_Value[0];
  tmp_1[13] = skipper_lqi_P.reference_const_Value[1];
  tmp_1[14] = skipper_lqi_P.reference_const_Value[2];
  tmp_2[12] = tmp_0[0];
  tmp_2[13] = tmp_0[1];
  tmp_2[14] = tmp_0[2];
  std::memcpy(&tmp_1[0], &skipper_lqi_P.d[0], 12U * sizeof(real_T));
  std::memcpy(&tmp_2[0], &x[0], 12U * sizeof(real_T));
  std::memcpy(&tmp_3[0], &skipper_lqi_DW.UnitDelay1_DSTATE[0], 12U * sizeof
              (real_T));
  tmp_3[12] = skipper_lqi_P.dummy_zeros_Value[0];
  tmp_3[13] = skipper_lqi_P.dummy_zeros_Value[1];
  tmp_3[14] = skipper_lqi_P.dummy_zeros_Value[2];
  for (i = 0; i < 15; i++) {
    /* DiscreteIntegrator: '<Root>/Discrete Integrator' incorporates:
     *  Sum: '<Root>/state_derivative_sum'
     */
    rtb_lower_gimbal_angle = ((tmp_1[i] + tmp_2[i]) + tmp_3[i]) *
      skipper_lqi_P.DiscreteIntegrator_gainval;
    state_with_error_tmp[i] = rtb_lower_gimbal_angle;

    /* DiscreteIntegrator: '<Root>/Discrete Integrator' */
    state_with_error[i] = skipper_lqi_DW.DiscreteIntegrator_DSTATE[i] +
      rtb_lower_gimbal_angle;
  }

  /* Gain: '<Root>/lqi_gain' */
  for (i = 0; i < 12; i++) {
    x[i] = -skipper_lqi_P.K2[i];
  }

  /* Gain: '<Root>/lqr_gain' */
  for (i = 0; i < 48; i++) {
    tmp_4[i] = -skipper_lqi_P.K1[i];
  }

  for (i = 0; i < 4; i++) {
    /* Gain: '<Root>/lqr_gain' */
    x_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      x_0 += tmp_4[(i_0 << 2) + i] * state_with_error[i_0];
    }

    /* Saturate: '<S3>/saturator' incorporates:
     *  Constant: '<Root>/ff_control'
     *  Gain: '<Root>/lqi_gain'
     *  Gain: '<Root>/lqr_gain'
     *  Sum: '<Root>/control_sum'
     */
    rtb_lower_gimbal_angle = (((x[i + 4] * state_with_error[13] + x[i] *
      state_with_error[12]) + x[i + 8] * state_with_error[14]) + x_0) +
      skipper_lqi_P.Uff[i];
    rtb_thrust = skipper_lqi_P.saturator_LowerSat[i];
    rtb_counter_rotating = skipper_lqi_P.saturator_UpperSat[i];
    if (rtb_lower_gimbal_angle > rtb_counter_rotating) {
      rtb_saturator[i] = rtb_counter_rotating;
    } else if (rtb_lower_gimbal_angle < rtb_thrust) {
      rtb_saturator[i] = rtb_thrust;
    } else {
      rtb_saturator[i] = rtb_lower_gimbal_angle;
    }

    /* End of Saturate: '<S3>/saturator' */
  }

  /* RateLimiter: '<S3>/Rate Limiter3' */
  rtb_lower_gimbal_angle = rtb_saturator[0] - skipper_lqi_DW.PrevY;
  if (rtb_lower_gimbal_angle > skipper_lqi_P.RateLimiter3_RisingLim *
      skipper_lqi_period) {
    rtb_thrust = skipper_lqi_P.RateLimiter3_RisingLim * skipper_lqi_period +
      skipper_lqi_DW.PrevY;
  } else if (rtb_lower_gimbal_angle < skipper_lqi_P.RateLimiter3_FallingLim *
             skipper_lqi_period) {
    rtb_thrust = skipper_lqi_P.RateLimiter3_FallingLim * skipper_lqi_period +
      skipper_lqi_DW.PrevY;
  } else {
    rtb_thrust = rtb_saturator[0];
  }

  skipper_lqi_DW.PrevY = rtb_thrust;

  /* End of RateLimiter: '<S3>/Rate Limiter3' */

  /* RateLimiter: '<S3>/Rate Limiter2' */
  rtb_lower_gimbal_angle = rtb_saturator[1] - skipper_lqi_DW.PrevY_k;
  if (rtb_lower_gimbal_angle > skipper_lqi_P.RateLimiter2_RisingLim *
      skipper_lqi_period) {
    rtb_counter_rotating = skipper_lqi_P.RateLimiter2_RisingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_k;
  } else if (rtb_lower_gimbal_angle < skipper_lqi_P.RateLimiter2_FallingLim *
             skipper_lqi_period) {
    rtb_counter_rotating = skipper_lqi_P.RateLimiter2_FallingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_k;
  } else {
    rtb_counter_rotating = rtb_saturator[1];
  }

  skipper_lqi_DW.PrevY_k = rtb_counter_rotating;

  /* End of RateLimiter: '<S3>/Rate Limiter2' */

  /* RateLimiter: '<S3>/Rate Limiter1' */
  rtb_lower_gimbal_angle = rtb_saturator[2] - skipper_lqi_DW.PrevY_b;
  if (rtb_lower_gimbal_angle > skipper_lqi_P.RateLimiter1_RisingLim *
      skipper_lqi_period) {
    rtb_upper_gimbal_angle = skipper_lqi_P.RateLimiter1_RisingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_b;
  } else if (rtb_lower_gimbal_angle < skipper_lqi_P.RateLimiter1_FallingLim *
             skipper_lqi_period) {
    rtb_upper_gimbal_angle = skipper_lqi_P.RateLimiter1_FallingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_b;
  } else {
    rtb_upper_gimbal_angle = rtb_saturator[2];
  }

  skipper_lqi_DW.PrevY_b = rtb_upper_gimbal_angle;

  /* End of RateLimiter: '<S3>/Rate Limiter1' */

  /* RateLimiter: '<S3>/Rate Limiter' */
  rtb_lower_gimbal_angle = rtb_saturator[3] - skipper_lqi_DW.PrevY_l;
  if (rtb_lower_gimbal_angle > skipper_lqi_P.RateLimiter_RisingLim *
      skipper_lqi_period) {
    rtb_lower_gimbal_angle = skipper_lqi_P.RateLimiter_RisingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_l;
  } else if (rtb_lower_gimbal_angle < skipper_lqi_P.RateLimiter_FallingLim *
             skipper_lqi_period) {
    rtb_lower_gimbal_angle = skipper_lqi_P.RateLimiter_FallingLim *
      skipper_lqi_period + skipper_lqi_DW.PrevY_l;
  } else {
    rtb_lower_gimbal_angle = rtb_saturator[3];
  }

  skipper_lqi_DW.PrevY_l = rtb_lower_gimbal_angle;

  /* End of RateLimiter: '<S3>/Rate Limiter' */
  for (i = 0; i < 12; i++) {
    /* Gain: '<Root>/output_gain' */
    x_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      x_0 += skipper_lqi_P.C[12 * i_0 + i] * state_with_error[i_0];
    }

    /* Sum: '<Root>/Sum2' incorporates:
     *  Gain: '<Root>/output_gain'
     *  Gain: '<Root>/passthrough_gain'
     *  SignalConversion generated from: '<Root>/input_gain'
     */
    rtb_Akxhatkk1[i] = (((skipper_lqi_P.D[i + 12] * rtb_counter_rotating +
                          skipper_lqi_P.D[i] * rtb_thrust) + skipper_lqi_P.D[i +
                         24] * rtb_upper_gimbal_angle) + skipper_lqi_P.D[i + 36]
                        * rtb_lower_gimbal_angle) + x_0;
  }

  /* Outport: '<Root>/Out1' */
  skipper_lqi_Y.altitude = rtb_Akxhatkk1[0];

  /* Outport: '<Root>/Out2' */
  skipper_lqi_Y.crossrange = rtb_Akxhatkk1[1];

  /* Outport: '<Root>/Out3' */
  skipper_lqi_Y.downrange = rtb_Akxhatkk1[2];

  /* Outport: '<Root>/Out4' */
  skipper_lqi_Y.lateral_vel = rtb_Akxhatkk1[3];

  /* Outport: '<Root>/Out5' */
  skipper_lqi_Y.longitudinal_vel = rtb_Akxhatkk1[4];

  /* Outport: '<Root>/Out6' */
  skipper_lqi_Y.directional_vel = rtb_Akxhatkk1[5];

  /* Outport: '<Root>/Out7' incorporates:
   *  Gain: '<Root>/Gain12'
   */
  skipper_lqi_Y.roll = skipper_lqi_P.Gain12_Gain * rtb_Akxhatkk1[6];

  /* Outport: '<Root>/Out8' incorporates:
   *  Gain: '<Root>/Gain11'
   */
  skipper_lqi_Y.pitch = skipper_lqi_P.Gain11_Gain * rtb_Akxhatkk1[7];

  /* Outport: '<Root>/Out9' incorporates:
   *  Gain: '<Root>/Gain10'
   */
  skipper_lqi_Y.yaw = skipper_lqi_P.Gain10_Gain * rtb_Akxhatkk1[8];

  /* Outport: '<Root>/Out10' incorporates:
   *  Gain: '<Root>/Gain9'
   */
  skipper_lqi_Y.lateral_rate = skipper_lqi_P.Gain9_Gain * rtb_Akxhatkk1[9];

  /* Outport: '<Root>/Out11' incorporates:
   *  Gain: '<Root>/Gain8'
   */
  skipper_lqi_Y.longitudinal_rate = skipper_lqi_P.Gain8_Gain * rtb_Akxhatkk1[10];

  /* Outport: '<Root>/Out12' incorporates:
   *  Gain: '<Root>/Gain7'
   */
  skipper_lqi_Y.directional_rate = skipper_lqi_P.Gain7_Gain * rtb_Akxhatkk1[11];

  /* Outport: '<Root>/Out13' */
  skipper_lqi_Y.thrust = rtb_thrust;

  /* Outport: '<Root>/Out14' */
  skipper_lqi_Y.reaction_torque = rtb_counter_rotating;

  /* Outport: '<Root>/Out15' incorporates:
   *  Gain: '<Root>/Gain5'
   */
  skipper_lqi_Y.upper_gimbal_angle = skipper_lqi_P.Gain5_Gain *
    rtb_upper_gimbal_angle;

  /* Outport: '<Root>/Out16' incorporates:
   *  Gain: '<Root>/Gain6'
   */
  skipper_lqi_Y.lower_gimbal_angle = skipper_lqi_P.Gain6_Gain *
    rtb_lower_gimbal_angle;

  /* Outport: '<Root>/Out17' */
  skipper_lqi_Y.altitude_error = state_with_error[12];

  /* Outport: '<Root>/Out18' */
  skipper_lqi_Y.crossrange_error = state_with_error[13];

  /* Outport: '<Root>/Out19' */
  skipper_lqi_Y.downrange_error = state_with_error[14];
  if ((&skipper_lqi_M)->Timing.TaskCounters.TID[1] == 0) {
    for (i = 0; i < 12; i++) {
      /* Gain: '<S1>/Output' incorporates:
       *  RandomNumber: '<S1>/White Noise'
       */
      skipper_lqi_B.Output[i] = std::sqrt
        (skipper_lqi_P.BandLimitedWhiteNoise_Cov[i]) / 0.31622776601683794 *
        skipper_lqi_DW.NextOutput;
    }
  }

  /* Delay: '<S2>/MemoryX' incorporates:
   *  Constant: '<S2>/X0'
   */
  if (skipper_lqi_DW.icLoad) {
    std::memcpy(&skipper_lqi_DW.MemoryX_DSTATE[0], &skipper_lqi_P.X0_Value[0],
                12U * sizeof(real_T));
  }

  for (i = 0; i < 12; i++) {
    /* Sum: '<Root>/Sum' */
    rtb_Akxhatkk1[i] += skipper_lqi_B.Output[i];
  }

  /* Outputs for Enabled SubSystem: '<S31>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S57>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S24>/MeasurementUpdate' incorporates:
   *  EnablePort: '<S55>/Enable'
   */
  /* Constant: '<S2>/Enable' */
  if (skipper_lqi_P.Enable_Value) {
    skipper_lqi_DW.MeasurementUpdate_MODE = true;
    for (i = 0; i < 12; i++) {
      /* Product: '<S55>/C[k]*xhat[k|k-1]' incorporates:
       *  Constant: '<S2>/C'
       */
      x_0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        /* Product: '<S57>/Product' incorporates:
         *  Constant: '<S2>/C'
         *  Delay: '<S2>/MemoryX'
         */
        x_0 += skipper_lqi_P.C_Value[12 * i_0 + i] *
          skipper_lqi_DW.MemoryX_DSTATE[i_0];
      }

      x[i] = x_0;

      /* End of Product: '<S55>/C[k]*xhat[k|k-1]' */

      /* Sum: '<S55>/Sum' incorporates:
       *  Constant: '<S2>/D'
       *  Product: '<S55>/D[k]*u[k]'
       *  SignalConversion generated from: '<Root>/input_gain'
       *  Sum: '<S55>/Add1'
       */
      rtb_Akxhatkk1_0[i] = rtb_Akxhatkk1[i] - ((((skipper_lqi_P.D_Value[i + 12] *
        rtb_counter_rotating + skipper_lqi_P.D_Value[i] * rtb_thrust) +
        skipper_lqi_P.D_Value[i + 24] * rtb_upper_gimbal_angle) +
        skipper_lqi_P.D_Value[i + 36] * rtb_lower_gimbal_angle) + x_0);
    }

    skipper_lqi_DW.EnabledSubsystem_MODE = true;
    for (i = 0; i < 12; i++) {
      /* Product: '<S55>/Product3' incorporates:
       *  Constant: '<S4>/KalmanGainL'
       */
      x_0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        x_0 += skipper_lqi_P.KalmanGainL_Value[12 * i_0 + i] *
          rtb_Akxhatkk1_0[i_0];
      }

      skipper_lqi_B.Product3[i] = x_0;

      /* End of Product: '<S55>/Product3' */

      /* Sum: '<S57>/Add1' incorporates:
       *  Product: '<S57>/Product'
       */
      rtb_Akxhatkk1_1[i] = rtb_Akxhatkk1[i] - x[i];
    }

    for (i = 0; i < 12; i++) {
      /* Product: '<S57>/Product2' incorporates:
       *  Constant: '<S4>/KalmanGainM'
       */
      x_0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        x_0 += skipper_lqi_P.KalmanGainM_Value[12 * i_0 + i] *
          rtb_Akxhatkk1_1[i_0];
      }

      skipper_lqi_B.Product2[i] = x_0;

      /* End of Product: '<S57>/Product2' */
    }
  } else {
    if (skipper_lqi_DW.MeasurementUpdate_MODE) {
      for (i = 0; i < 12; i++) {
        /* Disable for Product: '<S55>/Product3' incorporates:
         *  Outport: '<S55>/L*(y[k]-yhat[k|k-1])'
         */
        skipper_lqi_B.Product3[i] = skipper_lqi_P.Lykyhatkk1_Y0;
      }

      skipper_lqi_DW.MeasurementUpdate_MODE = false;
    }

    if (skipper_lqi_DW.EnabledSubsystem_MODE) {
      for (i = 0; i < 12; i++) {
        /* Disable for Product: '<S57>/Product2' incorporates:
         *  Outport: '<S57>/deltax'
         */
        skipper_lqi_B.Product2[i] = skipper_lqi_P.deltax_Y0;
      }

      skipper_lqi_DW.EnabledSubsystem_MODE = false;
    }
  }

  /* End of Constant: '<S2>/Enable' */
  /* End of Outputs for SubSystem: '<S24>/MeasurementUpdate' */
  /* End of Outputs for SubSystem: '<S31>/Enabled Subsystem' */
  for (i = 0; i < 12; i++) {
    /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
     *  Delay: '<S2>/MemoryX'
     *  Sum: '<S31>/Add'
     */
    skipper_lqi_DW.UnitDelay_DSTATE[i] = skipper_lqi_B.Product2[i] +
      skipper_lqi_DW.MemoryX_DSTATE[i];

    /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
     *  Gain: '<Root>/input_gain'
     *  SignalConversion generated from: '<Root>/input_gain'
     */
    skipper_lqi_DW.UnitDelay1_DSTATE[i] = ((skipper_lqi_P.B[i + 12] *
      rtb_counter_rotating + skipper_lqi_P.B[i] * rtb_thrust) +
      skipper_lqi_P.B[i + 24] * rtb_upper_gimbal_angle) + skipper_lqi_P.B[i + 36]
      * rtb_lower_gimbal_angle;
  }

  /* Update for DiscreteIntegrator: '<Root>/Discrete Integrator' */
  for (i = 0; i < 15; i++) {
    skipper_lqi_DW.DiscreteIntegrator_DSTATE[i] = state_with_error[i] +
      state_with_error_tmp[i];
  }

  if ((&skipper_lqi_M)->Timing.TaskCounters.TID[1] == 0) {
    /* Update for RandomNumber: '<S1>/White Noise' */
    skipper_lqi_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw_snf
      (&skipper_lqi_DW.RandSeed) * skipper_lqi_P.WhiteNoise_StdDev +
      skipper_lqi_P.WhiteNoise_Mean;
  }

  /* Update for Delay: '<S2>/MemoryX' */
  skipper_lqi_DW.icLoad = false;
  for (i = 0; i < 12; i++) {
    /* Product: '<S24>/B[k]*u[k]' incorporates:
     *  Constant: '<S2>/B'
     *  SignalConversion generated from: '<Root>/input_gain'
     */
    x[i] = ((skipper_lqi_P.B_Value[i + 12] * rtb_counter_rotating +
             skipper_lqi_P.B_Value[i] * rtb_thrust) + skipper_lqi_P.B_Value[i +
            24] * rtb_upper_gimbal_angle) + skipper_lqi_P.B_Value[i + 36] *
      rtb_lower_gimbal_angle;

    /* Product: '<S24>/A[k]*xhat[k|k-1]' incorporates:
     *  Constant: '<S2>/A'
     *  Delay: '<S2>/MemoryX'
     */
    x_0 = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      x_0 += skipper_lqi_P.A_Value[12 * i_0 + i] *
        skipper_lqi_DW.MemoryX_DSTATE[i_0];
    }

    rtb_Akxhatkk1[i] = x_0;

    /* End of Product: '<S24>/A[k]*xhat[k|k-1]' */
  }

  /* Update for Delay: '<S2>/MemoryX' incorporates:
   *  Sum: '<S24>/Add'
   */
  for (i = 0; i < 12; i++) {
    skipper_lqi_DW.MemoryX_DSTATE[i] = (x[i] + rtb_Akxhatkk1[i]) +
      skipper_lqi_B.Product3[i];
  }

  rate_scheduler((&skipper_lqi_M));
}

/* Model initialize function */
void skipper_lqi::initialize()
{
  /* Start for Constant: '<Root>/initial_state' */
  std::memcpy(&skipper_lqi_B.initial_state[0],
              &skipper_lqi_P.initial_state_Value[0], 12U * sizeof(real_T));

  /* Start for Constant: '<Root>/initial_error' */
  skipper_lqi_B.initial_error[0] = skipper_lqi_P.initial_error_Value[0];
  skipper_lqi_B.initial_error[1] = skipper_lqi_P.initial_error_Value[1];
  skipper_lqi_B.initial_error[2] = skipper_lqi_P.initial_error_Value[2];

  {
    real_T tmp;
    int32_T i;
    int32_T t;
    uint32_T tseed;
    for (i = 0; i < 12; i++) {
      /* InitializeConditions for UnitDelay: '<Root>/Unit Delay' */
      skipper_lqi_DW.UnitDelay_DSTATE[i] =
        skipper_lqi_P.UnitDelay_InitialCondition;

      /* InitializeConditions for UnitDelay: '<Root>/Unit Delay1' */
      skipper_lqi_DW.UnitDelay1_DSTATE[i] =
        skipper_lqi_P.UnitDelay1_InitialCondition;

      /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' */
      skipper_lqi_DW.DiscreteIntegrator_DSTATE[i] =
        skipper_lqi_B.initial_state[i];
    }

    /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete Integrator' */
    skipper_lqi_DW.DiscreteIntegrator_DSTATE[12] = skipper_lqi_B.initial_error[0];
    skipper_lqi_DW.DiscreteIntegrator_DSTATE[13] = skipper_lqi_B.initial_error[1];
    skipper_lqi_DW.DiscreteIntegrator_DSTATE[14] = skipper_lqi_B.initial_error[2];

    /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter3' */
    skipper_lqi_DW.PrevY = skipper_lqi_P.RateLimiter3_IC;

    /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter2' */
    skipper_lqi_DW.PrevY_k = skipper_lqi_P.RateLimiter2_IC;

    /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter1' */
    skipper_lqi_DW.PrevY_b = skipper_lqi_P.RateLimiter1_IC;

    /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter' */
    skipper_lqi_DW.PrevY_l = skipper_lqi_P.RateLimiter_IC;

    /* InitializeConditions for RandomNumber: '<S1>/White Noise' */
    tmp = std::floor(skipper_lqi_P.BandLimitedWhiteNoise_seed);
    if (std::isnan(tmp) || std::isinf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = std::fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) +
               static_cast<uint32_T>(t)) << 16U) + static_cast<uint32_T>(t)) +
      static_cast<uint32_T>(i);
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    skipper_lqi_DW.RandSeed = tseed;
    skipper_lqi_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw_snf
      (&skipper_lqi_DW.RandSeed) * skipper_lqi_P.WhiteNoise_StdDev +
      skipper_lqi_P.WhiteNoise_Mean;

    /* End of InitializeConditions for RandomNumber: '<S1>/White Noise' */

    /* InitializeConditions for Delay: '<S2>/MemoryX' */
    skipper_lqi_DW.icLoad = true;

    /* SystemInitialize for Enabled SubSystem: '<S31>/Enabled Subsystem' */
    /* SystemInitialize for Enabled SubSystem: '<S24>/MeasurementUpdate' */
    for (i = 0; i < 12; i++) {
      /* SystemInitialize for Product: '<S55>/Product3' incorporates:
       *  Outport: '<S55>/L*(y[k]-yhat[k|k-1])'
       */
      skipper_lqi_B.Product3[i] = skipper_lqi_P.Lykyhatkk1_Y0;

      /* SystemInitialize for Product: '<S57>/Product2' incorporates:
       *  Outport: '<S57>/deltax'
       */
      skipper_lqi_B.Product2[i] = skipper_lqi_P.deltax_Y0;
    }

    /* End of SystemInitialize for SubSystem: '<S24>/MeasurementUpdate' */
    /* End of SystemInitialize for SubSystem: '<S31>/Enabled Subsystem' */
  }
}

/* Model terminate function */
void skipper_lqi::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
skipper_lqi::skipper_lqi() :
  skipper_lqi_Y(),
  skipper_lqi_B(),
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
