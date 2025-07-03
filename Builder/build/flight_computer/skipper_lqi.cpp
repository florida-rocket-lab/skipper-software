/*
 * skipper_lqi.cpp
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi".
 *
 * Model version              : 1.116
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C++ source code generated on : Tue Jun 24 13:39:23 2025
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
#include "skipper_lqi_private.h"

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
void skipper_lqi::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3]{
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3]{
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t { rtsiGetT(si) };

  time_T tnew { rtsiGetSolverStopTime(si) };

  time_T h { rtsiGetStepSize(si) };

  real_T *x { rtsiGetContStates(si) };

  ODE3_IntgData *id { static_cast<ODE3_IntgData *>(rtsiGetSolverData(si)) };

  real_T *y { id->y };

  real_T *f0 { id->f[0] };

  real_T *f1 { id->f[1] };

  real_T *f2 { id->f[2] };

  real_T hB[3];
  int_T i;
  int_T nXc { 15 };

  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) std::memcpy(y, x,
                     static_cast<uint_T>(nXc)*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  skipper_lqi_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  this->step();
  skipper_lqi_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  this->step();
  skipper_lqi_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void skipper_lqi::step()
{
  if (rtmIsMajorTimeStep((&skipper_lqi_M))) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&(&skipper_lqi_M)->solverInfo,(((&skipper_lqi_M)
      ->Timing.clockTick0+1)*(&skipper_lqi_M)->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&skipper_lqi_M))) {
    (&skipper_lqi_M)->Timing.t[0] = rtsiGetT(&(&skipper_lqi_M)->solverInfo);
  }

  {
    real_T tmp_1[15];
    real_T tmp_2[15];
    real_T tmp_3[15];
    real_T rtb_input_gain[12];
    real_T tmp[12];
    real_T rtb_saturator[4];
    real_T tmp_0[3];
    real_T u0;
    real_T u1;
    real_T u2;
    int32_T i;
    int32_T i_0;
    boolean_T limitedCache;

    /* SignalConversion generated from: '<Root>/state_integrator' incorporates:
     *  Constant: '<Root>/initial_error'
     */
    std::memset(&skipper_lqi_B.initial_conditions[0], 0, 15U * sizeof(real_T));

    /* Integrator: '<Root>/state_integrator' */
    if (skipper_lqi_DW.state_integrator_DWORK1) {
      std::memcpy(&skipper_lqi_X.state_integrator_CSTATE[0],
                  &skipper_lqi_B.initial_conditions[0], 15U * sizeof(real_T));
    }

    for (i = 0; i < 4; i++) {
      /* Gain: '<Root>/lqr_gain' incorporates:
       *  Integrator: '<Root>/state_integrator'
       */
      u0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        u0 += skipper_lqi_ConstP.lqr_gain_Gain[(i_0 << 2) + i] *
          skipper_lqi_X.state_integrator_CSTATE[i_0];
      }

      /* Saturate: '<S1>/saturator' incorporates:
       *  Constant: '<Root>/ff_control'
       *  Gain: '<Root>/lqi_gain'
       *  Gain: '<Root>/lqr_gain'
       *  Integrator: '<Root>/state_integrator'
       *  Sum: '<Root>/control_sum'
       */
      u0 = (((skipper_lqi_ConstP.lqi_gain_Gain[i + 4] *
              skipper_lqi_X.state_integrator_CSTATE[13] +
              skipper_lqi_ConstP.lqi_gain_Gain[i] *
              skipper_lqi_X.state_integrator_CSTATE[12]) +
             skipper_lqi_ConstP.lqi_gain_Gain[i + 8] *
             skipper_lqi_X.state_integrator_CSTATE[14]) + u0) +
        skipper_lqi_ConstP.ff_control_Value[i];
      u1 = skipper_lqi_ConstP.saturator_LowerSat[i];
      u2 = skipper_lqi_ConstP.saturator_UpperSat[i];
      if (u0 > u2) {
        rtb_saturator[i] = u2;
      } else if (u0 < u1) {
        rtb_saturator[i] = u1;
      } else {
        rtb_saturator[i] = u0;
      }

      /* End of Saturate: '<S1>/saturator' */
    }

    /* RateLimiter: '<S1>/Rate Limiter3' */
    if (skipper_lqi_DW.LastMajorTime == (rtInf)) {
      /* RateLimiter: '<S1>/Rate Limiter3' */
      skipper_lqi_B.thrust = rtb_saturator[0];
    } else {
      u0 = (&skipper_lqi_M)->Timing.t[0];
      u1 = u0 - skipper_lqi_DW.LastMajorTime;
      if (skipper_lqi_DW.LastMajorTime == u0) {
        if (skipper_lqi_DW.PrevLimited) {
          /* RateLimiter: '<S1>/Rate Limiter3' */
          skipper_lqi_B.thrust = skipper_lqi_DW.PrevY;
        } else {
          /* RateLimiter: '<S1>/Rate Limiter3' */
          skipper_lqi_B.thrust = rtb_saturator[0];
        }
      } else {
        u2 = u1 * (rtInf);
        u0 = rtb_saturator[0] - skipper_lqi_DW.PrevY;
        if (u0 > u2) {
          /* RateLimiter: '<S1>/Rate Limiter3' */
          skipper_lqi_B.thrust = skipper_lqi_DW.PrevY + u2;
          limitedCache = true;
        } else {
          u1 *= (rtMinusInf);
          if (u0 < u1) {
            /* RateLimiter: '<S1>/Rate Limiter3' */
            skipper_lqi_B.thrust = skipper_lqi_DW.PrevY + u1;
            limitedCache = true;
          } else {
            /* RateLimiter: '<S1>/Rate Limiter3' */
            skipper_lqi_B.thrust = rtb_saturator[0];
            limitedCache = false;
          }
        }

        if (rtsiIsModeUpdateTimeStep(&(&skipper_lqi_M)->solverInfo)) {
          skipper_lqi_DW.PrevLimited = limitedCache;
        }
      }
    }

    /* End of RateLimiter: '<S1>/Rate Limiter3' */

    /* RateLimiter: '<S1>/Rate Limiter2' */
    if (skipper_lqi_DW.LastMajorTime_f == (rtInf)) {
      /* RateLimiter: '<S1>/Rate Limiter2' */
      skipper_lqi_B.counter_rotating = rtb_saturator[1];
    } else {
      u0 = (&skipper_lqi_M)->Timing.t[0];
      u1 = u0 - skipper_lqi_DW.LastMajorTime_f;
      if (skipper_lqi_DW.LastMajorTime_f == u0) {
        if (skipper_lqi_DW.PrevLimited_p) {
          /* RateLimiter: '<S1>/Rate Limiter2' */
          skipper_lqi_B.counter_rotating = skipper_lqi_DW.PrevY_k;
        } else {
          /* RateLimiter: '<S1>/Rate Limiter2' */
          skipper_lqi_B.counter_rotating = rtb_saturator[1];
        }
      } else {
        u2 = u1 * (rtInf);
        u0 = rtb_saturator[1] - skipper_lqi_DW.PrevY_k;
        if (u0 > u2) {
          /* RateLimiter: '<S1>/Rate Limiter2' */
          skipper_lqi_B.counter_rotating = skipper_lqi_DW.PrevY_k + u2;
          limitedCache = true;
        } else {
          u1 *= (rtMinusInf);
          if (u0 < u1) {
            /* RateLimiter: '<S1>/Rate Limiter2' */
            skipper_lqi_B.counter_rotating = skipper_lqi_DW.PrevY_k + u1;
            limitedCache = true;
          } else {
            /* RateLimiter: '<S1>/Rate Limiter2' */
            skipper_lqi_B.counter_rotating = rtb_saturator[1];
            limitedCache = false;
          }
        }

        if (rtsiIsModeUpdateTimeStep(&(&skipper_lqi_M)->solverInfo)) {
          skipper_lqi_DW.PrevLimited_p = limitedCache;
        }
      }
    }

    /* End of RateLimiter: '<S1>/Rate Limiter2' */

    /* RateLimiter: '<S1>/Rate Limiter1' */
    if (skipper_lqi_DW.LastMajorTime_o == (rtInf)) {
      /* RateLimiter: '<S1>/Rate Limiter1' */
      skipper_lqi_B.upper_gimbal_angle = rtb_saturator[2];
    } else {
      u0 = (&skipper_lqi_M)->Timing.t[0];
      u1 = u0 - skipper_lqi_DW.LastMajorTime_o;
      if (skipper_lqi_DW.LastMajorTime_o == u0) {
        if (skipper_lqi_DW.PrevLimited_n) {
          /* RateLimiter: '<S1>/Rate Limiter1' */
          skipper_lqi_B.upper_gimbal_angle = skipper_lqi_DW.PrevY_b;
        } else {
          /* RateLimiter: '<S1>/Rate Limiter1' */
          skipper_lqi_B.upper_gimbal_angle = rtb_saturator[2];
        }
      } else {
        u2 = u1 * (rtInf);
        u0 = rtb_saturator[2] - skipper_lqi_DW.PrevY_b;
        if (u0 > u2) {
          /* RateLimiter: '<S1>/Rate Limiter1' */
          skipper_lqi_B.upper_gimbal_angle = skipper_lqi_DW.PrevY_b + u2;
          limitedCache = true;
        } else {
          u1 *= (rtMinusInf);
          if (u0 < u1) {
            /* RateLimiter: '<S1>/Rate Limiter1' */
            skipper_lqi_B.upper_gimbal_angle = skipper_lqi_DW.PrevY_b + u1;
            limitedCache = true;
          } else {
            /* RateLimiter: '<S1>/Rate Limiter1' */
            skipper_lqi_B.upper_gimbal_angle = rtb_saturator[2];
            limitedCache = false;
          }
        }

        if (rtsiIsModeUpdateTimeStep(&(&skipper_lqi_M)->solverInfo)) {
          skipper_lqi_DW.PrevLimited_n = limitedCache;
        }
      }
    }

    /* End of RateLimiter: '<S1>/Rate Limiter1' */

    /* RateLimiter: '<S1>/Rate Limiter' */
    if (skipper_lqi_DW.LastMajorTime_m == (rtInf)) {
      /* RateLimiter: '<S1>/Rate Limiter' */
      skipper_lqi_B.lower_gimbal_angle = rtb_saturator[3];
    } else {
      u0 = (&skipper_lqi_M)->Timing.t[0];
      u1 = u0 - skipper_lqi_DW.LastMajorTime_m;
      if (skipper_lqi_DW.LastMajorTime_m == u0) {
        if (skipper_lqi_DW.PrevLimited_i) {
          /* RateLimiter: '<S1>/Rate Limiter' */
          skipper_lqi_B.lower_gimbal_angle = skipper_lqi_DW.PrevY_l;
        } else {
          /* RateLimiter: '<S1>/Rate Limiter' */
          skipper_lqi_B.lower_gimbal_angle = rtb_saturator[3];
        }
      } else {
        u2 = u1 * (rtInf);
        u0 = rtb_saturator[3] - skipper_lqi_DW.PrevY_l;
        if (u0 > u2) {
          /* RateLimiter: '<S1>/Rate Limiter' */
          skipper_lqi_B.lower_gimbal_angle = skipper_lqi_DW.PrevY_l + u2;
          limitedCache = true;
        } else {
          u1 *= (rtMinusInf);
          if (u0 < u1) {
            /* RateLimiter: '<S1>/Rate Limiter' */
            skipper_lqi_B.lower_gimbal_angle = skipper_lqi_DW.PrevY_l + u1;
            limitedCache = true;
          } else {
            /* RateLimiter: '<S1>/Rate Limiter' */
            skipper_lqi_B.lower_gimbal_angle = rtb_saturator[3];
            limitedCache = false;
          }
        }

        if (rtsiIsModeUpdateTimeStep(&(&skipper_lqi_M)->solverInfo)) {
          skipper_lqi_DW.PrevLimited_i = limitedCache;
        }
      }
    }

    /* End of RateLimiter: '<S1>/Rate Limiter' */
    for (i = 0; i < 12; i++) {
      /* Gain: '<Root>/output_gain' incorporates:
       *  Integrator: '<Root>/state_integrator'
       */
      u0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        u0 += skipper_lqi_ConstP.output_gain_Gain[12 * i_0 + i] *
          skipper_lqi_X.state_integrator_CSTATE[i_0];
      }

      /* Sum: '<Root>/Sum2' incorporates:
       *  Gain: '<Root>/output_gain'
       *  Gain: '<Root>/passthrough_gain'
       *  SignalConversion generated from: '<Root>/input_gain'
       */
      rtb_input_gain[i] = (((0.0 * skipper_lqi_B.thrust + 0.0 *
        skipper_lqi_B.counter_rotating) + 0.0 * skipper_lqi_B.upper_gimbal_angle)
                           + 0.0 * skipper_lqi_B.lower_gimbal_angle) + u0;
    }

    /* Outport: '<Root>/Out1' */
    skipper_lqi_Y.altitude = rtb_input_gain[0];

    /* Outport: '<Root>/Out2' */
    skipper_lqi_Y.crossrange = rtb_input_gain[1];

    /* Outport: '<Root>/Out3' */
    skipper_lqi_Y.downrange = rtb_input_gain[2];

    /* Outport: '<Root>/Out4' */
    skipper_lqi_Y.lateral_vel = rtb_input_gain[3];

    /* Outport: '<Root>/Out5' */
    skipper_lqi_Y.longitudinal_vel = rtb_input_gain[4];

    /* Outport: '<Root>/Out6' */
    skipper_lqi_Y.directional_vel = rtb_input_gain[5];

    /* Outport: '<Root>/Out7' incorporates:
     *  Gain: '<Root>/Gain12'
     */
    skipper_lqi_Y.roll = 57.295779513082323 * rtb_input_gain[6];

    /* Outport: '<Root>/Out8' incorporates:
     *  Gain: '<Root>/Gain11'
     */
    skipper_lqi_Y.pitch = 57.295779513082323 * rtb_input_gain[7];

    /* Outport: '<Root>/Out9' incorporates:
     *  Gain: '<Root>/Gain10'
     */
    skipper_lqi_Y.yaw = 57.295779513082323 * rtb_input_gain[8];

    /* Outport: '<Root>/Out10' incorporates:
     *  Gain: '<Root>/Gain9'
     */
    skipper_lqi_Y.lateral_rate = 57.295779513082323 * rtb_input_gain[9];

    /* Outport: '<Root>/Out11' incorporates:
     *  Gain: '<Root>/Gain8'
     */
    skipper_lqi_Y.longitudinal_rate = 57.295779513082323 * rtb_input_gain[10];

    /* Outport: '<Root>/Out12' incorporates:
     *  Gain: '<Root>/Gain7'
     */
    skipper_lqi_Y.directional_rate = 57.295779513082323 * rtb_input_gain[11];

    /* Outport: '<Root>/Out13' */
    skipper_lqi_Y.thrust = skipper_lqi_B.thrust;

    /* Outport: '<Root>/Out14' */
    skipper_lqi_Y.reaction_torque = skipper_lqi_B.counter_rotating;

    /* Outport: '<Root>/Out15' incorporates:
     *  Gain: '<Root>/Gain5'
     */
    skipper_lqi_Y.upper_gimbal_angle = 57.295779513082323 *
      skipper_lqi_B.upper_gimbal_angle;

    /* Outport: '<Root>/Out16' incorporates:
     *  Gain: '<Root>/Gain6'
     */
    skipper_lqi_Y.lower_gimbal_angle = 57.295779513082323 *
      skipper_lqi_B.lower_gimbal_angle;

    /* Outport: '<Root>/Out17' incorporates:
     *  Integrator: '<Root>/state_integrator'
     */
    skipper_lqi_Y.altitude_error = skipper_lqi_X.state_integrator_CSTATE[12];

    /* Outport: '<Root>/Out18' incorporates:
     *  Integrator: '<Root>/state_integrator'
     */
    skipper_lqi_Y.crossrange_error = skipper_lqi_X.state_integrator_CSTATE[13];

    /* Outport: '<Root>/Out19' incorporates:
     *  Integrator: '<Root>/state_integrator'
     */
    skipper_lqi_Y.downrange_error = skipper_lqi_X.state_integrator_CSTATE[14];

    /* Sum: '<Root>/feedback_sum' incorporates:
     *  Integrator: '<Root>/state_integrator'
     */
    std::memcpy(&rtb_input_gain[0], &skipper_lqi_X.state_integrator_CSTATE[0],
                12U * sizeof(real_T));

    /* Gain: '<Root>/state_gain' */
    for (i = 0; i < 12; i++) {
      u0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        u0 += skipper_lqi_ConstP.state_gain_Gain[12 * i_0 + i] *
          rtb_input_gain[i_0];
      }

      tmp[i] = u0;
    }

    /* End of Gain: '<Root>/state_gain' */

    /* Gain: '<Root>/extraction_gain' */
    for (i = 0; i < 3; i++) {
      u0 = 0.0;
      for (i_0 = 0; i_0 < 12; i_0++) {
        u0 += skipper_lqi_ConstP.extraction_gain_Gain[3 * i_0 + i] *
          rtb_input_gain[i_0];
      }

      tmp_0[i] = u0;
    }

    /* End of Gain: '<Root>/extraction_gain' */

    /* Sum: '<Root>/state_derivative_sum' incorporates:
     *  Constant: '<Root>/dummy_zeros'
     *  Constant: '<Root>/gravity_const'
     *  Constant: '<Root>/reference_const'
     *  Gain: '<Root>/input_gain'
     *  SignalConversion generated from: '<Root>/input_gain'
     */
    tmp_1[12] = 2.0;
    tmp_1[13] = 0.0;
    tmp_1[14] = 0.0;
    // hover at 2 m



    tmp_2[12] = tmp_0[0];
    tmp_2[13] = tmp_0[1];
    tmp_2[14] = tmp_0[2];
    for (i = 0; i < 12; i++) {
      tmp_1[i] = skipper_lqi_ConstP.gravity_const_Value[i];
      tmp_2[i] = tmp[i];
      tmp_3[i] = ((skipper_lqi_ConstP.input_gain_Gain[i + 12] *
                   skipper_lqi_B.counter_rotating +
                   skipper_lqi_ConstP.input_gain_Gain[i] * skipper_lqi_B.thrust)
                  + skipper_lqi_ConstP.input_gain_Gain[i + 24] *
                  skipper_lqi_B.upper_gimbal_angle) +
        skipper_lqi_ConstP.input_gain_Gain[i + 36] *
        skipper_lqi_B.lower_gimbal_angle;
    }

    tmp_3[12] = 0.0;
    tmp_3[13] = 0.0;
    tmp_3[14] = 0.0;
    for (i = 0; i < 15; i++) {
      /* Sum: '<Root>/state_derivative_sum' */
      skipper_lqi_B.state_with_error_derivative[i] = (tmp_1[i] + tmp_2[i]) +
        tmp_3[i];
    }
  }

  if (rtmIsMajorTimeStep((&skipper_lqi_M))) {
    real_T LastMajorTime_tmp;

    /* Update for Integrator: '<Root>/state_integrator' */
    skipper_lqi_DW.state_integrator_DWORK1 = false;

    /* Update for RateLimiter: '<S1>/Rate Limiter3' incorporates:
     *  RateLimiter: '<S1>/Rate Limiter'
     *  RateLimiter: '<S1>/Rate Limiter1'
     *  RateLimiter: '<S1>/Rate Limiter2'
     */
    skipper_lqi_DW.PrevY = skipper_lqi_B.thrust;
    LastMajorTime_tmp = (&skipper_lqi_M)->Timing.t[0];
    skipper_lqi_DW.LastMajorTime = LastMajorTime_tmp;

    /* Update for RateLimiter: '<S1>/Rate Limiter2' */
    skipper_lqi_DW.PrevY_k = skipper_lqi_B.counter_rotating;
    skipper_lqi_DW.LastMajorTime_f = LastMajorTime_tmp;

    /* Update for RateLimiter: '<S1>/Rate Limiter1' */
    skipper_lqi_DW.PrevY_b = skipper_lqi_B.upper_gimbal_angle;
    skipper_lqi_DW.LastMajorTime_o = LastMajorTime_tmp;

    /* Update for RateLimiter: '<S1>/Rate Limiter' */
    skipper_lqi_DW.PrevY_l = skipper_lqi_B.lower_gimbal_angle;
    skipper_lqi_DW.LastMajorTime_m = LastMajorTime_tmp;
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&skipper_lqi_M))) {
    rt_ertODEUpdateContinuousStates(&(&skipper_lqi_M)->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++(&skipper_lqi_M)->Timing.clockTick0;
    (&skipper_lqi_M)->Timing.t[0] = rtsiGetSolverStopTime(&(&skipper_lqi_M)
      ->solverInfo);

    {
      /* Update absolute timer for sample time: [6.0s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 6.0, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      (&skipper_lqi_M)->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void skipper_lqi::skipper_lqi_derivatives()
{
  XDot_skipper_lqi_T *_rtXdot;
  _rtXdot = ((XDot_skipper_lqi_T *) (&skipper_lqi_M)->derivs);

  /* Derivatives for Integrator: '<Root>/state_integrator' */
  std::memcpy(&_rtXdot->state_integrator_CSTATE[0],
              &skipper_lqi_B.state_with_error_derivative[0], 15U * sizeof(real_T));
}

/* Model initialize function */
void skipper_lqi::initialize()
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&skipper_lqi_M)->solverInfo, &(&skipper_lqi_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&skipper_lqi_M)->solverInfo, &rtmGetTPtr((&skipper_lqi_M)));
    rtsiSetStepSizePtr(&(&skipper_lqi_M)->solverInfo, &(&skipper_lqi_M)
                       ->Timing.stepSize0);
    rtsiSetdXPtr(&(&skipper_lqi_M)->solverInfo, &(&skipper_lqi_M)->derivs);
    rtsiSetContStatesPtr(&(&skipper_lqi_M)->solverInfo, (real_T **)
                         &(&skipper_lqi_M)->contStates);
    rtsiSetNumContStatesPtr(&(&skipper_lqi_M)->solverInfo, &(&skipper_lqi_M)
      ->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&skipper_lqi_M)->solverInfo,
      &(&skipper_lqi_M)->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&skipper_lqi_M)->solverInfo,
      &(&skipper_lqi_M)->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&skipper_lqi_M)->solverInfo,
      &(&skipper_lqi_M)->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&(&skipper_lqi_M)->solverInfo, (boolean_T**)
      &(&skipper_lqi_M)->contStateDisabled);
    rtsiSetErrorStatusPtr(&(&skipper_lqi_M)->solverInfo, (&rtmGetErrorStatus
      ((&skipper_lqi_M))));
    rtsiSetRTModelPtr(&(&skipper_lqi_M)->solverInfo, (&skipper_lqi_M));
  }

  rtsiSetSimTimeStep(&(&skipper_lqi_M)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&(&skipper_lqi_M)->solverInfo, false);
  rtsiSetIsContModeFrozen(&(&skipper_lqi_M)->solverInfo, false);
  (&skipper_lqi_M)->intgData.y = (&skipper_lqi_M)->odeY;
  (&skipper_lqi_M)->intgData.f[0] = (&skipper_lqi_M)->odeF[0];
  (&skipper_lqi_M)->intgData.f[1] = (&skipper_lqi_M)->odeF[1];
  (&skipper_lqi_M)->intgData.f[2] = (&skipper_lqi_M)->odeF[2];
  (&skipper_lqi_M)->contStates = ((X_skipper_lqi_T *) &skipper_lqi_X);
  (&skipper_lqi_M)->contStateDisabled = ((XDis_skipper_lqi_T *)
    &skipper_lqi_XDis);
  (&skipper_lqi_M)->Timing.tStart = (0.0);
  rtsiSetSolverData(&(&skipper_lqi_M)->solverInfo, static_cast<void *>
                    (&(&skipper_lqi_M)->intgData));
  rtsiSetSolverName(&(&skipper_lqi_M)->solverInfo,"ode3");
  rtmSetTPtr((&skipper_lqi_M), &(&skipper_lqi_M)->Timing.tArray[0]);
  (&skipper_lqi_M)->Timing.stepSize0 = 6.0;
  rtmSetFirstInitCond((&skipper_lqi_M), 1);

  /* InitializeConditions for Integrator: '<Root>/state_integrator' */
  if (rtmIsFirstInitCond((&skipper_lqi_M))) {
    std::memset(&skipper_lqi_X.state_integrator_CSTATE[0], 0, 15U * sizeof
                (real_T));
  }

  skipper_lqi_DW.state_integrator_DWORK1 = true;

  /* End of InitializeConditions for Integrator: '<Root>/state_integrator' */

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter3' */
  skipper_lqi_DW.LastMajorTime = (rtInf);

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter2' */
  skipper_lqi_DW.LastMajorTime_f = (rtInf);

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter1' */
  skipper_lqi_DW.LastMajorTime_o = (rtInf);

  /* InitializeConditions for RateLimiter: '<S1>/Rate Limiter' */
  skipper_lqi_DW.LastMajorTime_m = (rtInf);

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond((&skipper_lqi_M))) {
    rtmSetFirstInitCond((&skipper_lqi_M), 0);
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
  skipper_lqi_X(),
  skipper_lqi_XDis(),
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
