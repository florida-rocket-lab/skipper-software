/*
 * skipper_lqi_for_export.h
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi_for_export".
 *
 * Model version              : 1.121
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C++ source code generated on : Wed Jul 16 14:14:08 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#ifndef skipper_lqi_for_export_h_
#define skipper_lqi_for_export_h_
#include <cmath>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "skipper_lqi_for_export_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
struct DW_skipper_lqi_for_export_T {
  real_T UnitDelay_DSTATE[12];         /* '<Root>/Unit Delay' */
  real_T UnitDelay1_DSTATE[12];        /* '<Root>/Unit Delay1' */
  real_T DiscreteIntegrator_DSTATE[15];/* '<Root>/Discrete Integrator' */
  real_T PrevY;                        /* '<S1>/Rate Limiter3' */
  real_T PrevY_k;                      /* '<S1>/Rate Limiter2' */
  real_T PrevY_b;                      /* '<S1>/Rate Limiter1' */
  real_T PrevY_l;                      /* '<S1>/Rate Limiter' */
};

/* Invariant block signals (default storage) */
struct ConstB_skipper_lqi_for_export_T {
  real_T initial_error[3];             /* '<Root>/initial_error' */
  real_T initial_state[12];            /* '<Root>/initial_state' */
};

/* Constant parameters (default storage) */
struct ConstP_skipper_lqi_for_export_T {
  /* Expression: Uff
   * Referenced by: '<Root>/ff_control'
   */
  real_T ff_control_Value[4];

  /* Expression: d
   * Referenced by: '<Root>/gravity_const'
   */
  real_T gravity_const_Value[12];

  /* Expression: A
   * Referenced by: '<Root>/state_gain'
   */
  real_T state_gain_Gain[144];

  /* Expression: -E
   * Referenced by: '<Root>/extraction_gain'
   */
  real_T extraction_gain_Gain[36];

  /* Expression: C
   * Referenced by: '<Root>/output_gain'
   */
  real_T output_gain_Gain[144];

  /* Expression: -K2
   * Referenced by: '<Root>/lqi_gain'
   */
  real_T lqi_gain_Gain[12];

  /* Expression: -K1
   * Referenced by: '<Root>/lqr_gain'
   */
  real_T lqr_gain_Gain[48];

  /* Expression: [M*g*4 1 pi/22 pi/22]'
   * Referenced by: '<S1>/saturator'
   */
  real_T saturator_UpperSat[4];

  /* Expression: [0 -1 -pi/22 -pi/22]'
   * Referenced by: '<S1>/saturator'
   */
  real_T saturator_LowerSat[4];

  /* Expression: B
   * Referenced by: '<Root>/input_gain'
   */
  real_T input_gain_Gain[48];
};

/* External inputs (root inport signals with default storage) */
struct ExtU_skipper_lqi_for_export_T {
  real_T imu_state_in[12];             /* '<Root>/imu_state_in' */
  real_T reference[3];                 /* '<Root>/refeference_in' */
};

/* External outputs (root outports fed by signals with default storage) */
struct ExtY_skipper_lqi_for_export_T {
  real_T altitude;                     /* '<Root>/Out1' */
  real_T crossrange;                   /* '<Root>/Out2' */
  real_T control[4];                   /* '<Root>/control_commands_out' */
  real_T downrange;                    /* '<Root>/Out3' */
  real_T lateral_vel;                  /* '<Root>/Out4' */
  real_T longitudinal_vel;             /* '<Root>/Out5' */
  real_T directional_vel;              /* '<Root>/Out6' */
  real_T roll;                         /* '<Root>/Out7' */
  real_T pitch;                        /* '<Root>/Out8' */
  real_T yaw;                          /* '<Root>/Out9' */
  real_T lateral_rate;                 /* '<Root>/Out10' */
  real_T longitudinal_rate;            /* '<Root>/Out11' */
  real_T directional_rate;             /* '<Root>/Out12' */
  real_T thrust;                       /* '<Root>/Out13' */
  real_T reaction_torque;              /* '<Root>/Out14' */
  real_T upper_gimbal_angle;           /* '<Root>/Out15' */
  real_T lower_gimbal_angle;           /* '<Root>/Out16' */
  real_T altitude_error;               /* '<Root>/Out17' */
  real_T crossrange_error;             /* '<Root>/Out18' */
  real_T downrange_error;              /* '<Root>/Out19' */
};

/* Real-time Model Data Structure */
struct tag_RTM_skipper_lqi_for_expor_T {
  const char_T *errorStatus;
};

extern const ConstB_skipper_lqi_for_export_T skipper_lqi_for_export_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_skipper_lqi_for_export_T skipper_lqi_for_export_ConstP;

/* Class declaration for model skipper_lqi_for_export */
class skipper_lqi_for_export final
{
  /* public data and function members */
 public:
  /* Copy Constructor */
  skipper_lqi_for_export(skipper_lqi_for_export const&) = delete;

  /* Assignment Operator */
  skipper_lqi_for_export& operator= (skipper_lqi_for_export const&) & = delete;

  /* Move Constructor */
  skipper_lqi_for_export(skipper_lqi_for_export &&) = delete;

  /* Move Assignment Operator */
  skipper_lqi_for_export& operator= (skipper_lqi_for_export &&) = delete;

  /* Real-Time Model get method */
  RT_MODEL_skipper_lqi_for_expo_T * getRTM();

  /* External inputs */
  ExtU_skipper_lqi_for_export_T skipper_lqi_for_export_U;

  /* External outputs */
  ExtY_skipper_lqi_for_export_T skipper_lqi_for_export_Y;

  /* Initial conditions function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  static void terminate();

  /* Constructor */
  skipper_lqi_for_export();

  /* Destructor */
  ~skipper_lqi_for_export();

  /* private data and function members */
 private:
  /* Block states */
  DW_skipper_lqi_for_export_T skipper_lqi_for_export_DW;

  /* Real-Time Model */
  RT_MODEL_skipper_lqi_for_expo_T skipper_lqi_for_export_M;
};

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'skipper_lqi_for_export'
 * '<S1>'   : 'skipper_lqi_for_export/control_constraints'
 */
#endif                                 /* skipper_lqi_for_export_h_ */
