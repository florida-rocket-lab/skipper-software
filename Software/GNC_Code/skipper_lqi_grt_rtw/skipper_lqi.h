/*
 * skipper_lqi.h
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi".
 *
 * Model version              : 1.113
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C++ source code generated on : Wed Jun 18 17:25:28 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#ifndef skipper_lqi_h_
#define skipper_lqi_h_
#include <cmath>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "skipper_lqi_types.h"

extern "C"
{

#include "rtGetInf.h"

}

#include <cstring>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals (default storage) */
struct B_skipper_lqi_T {
  real_T TmpSignalConversionAtstate_inte[15];
  real_T thrust;                       /* '<S1>/Rate Limiter3' */
  real_T counter_rotating;             /* '<S1>/Rate Limiter2' */
  real_T upper_gimbal_angle;           /* '<S1>/Rate Limiter1' */
  real_T lower_gimbal_angle;           /* '<S1>/Rate Limiter' */
  real_T state_with_error_derivative[15];/* '<Root>/TARGET_IN' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_skipper_lqi_T {
  real_T PrevY;                        /* '<S1>/Rate Limiter3' */
  real_T LastMajorTime;                /* '<S1>/Rate Limiter3' */
  real_T PrevY_k;                      /* '<S1>/Rate Limiter2' */
  real_T LastMajorTime_f;              /* '<S1>/Rate Limiter2' */
  real_T PrevY_b;                      /* '<S1>/Rate Limiter1' */
  real_T LastMajorTime_o;              /* '<S1>/Rate Limiter1' */
  real_T PrevY_l;                      /* '<S1>/Rate Limiter' */
  real_T LastMajorTime_m;              /* '<S1>/Rate Limiter' */
  boolean_T state_integrator_DWORK1;   /* '<Root>/state_integrator' */
  boolean_T PrevLimited;               /* '<S1>/Rate Limiter3' */
  boolean_T PrevLimited_p;             /* '<S1>/Rate Limiter2' */
  boolean_T PrevLimited_n;             /* '<S1>/Rate Limiter1' */
  boolean_T PrevLimited_i;             /* '<S1>/Rate Limiter' */
};

/* Continuous states (default storage) */
struct X_skipper_lqi_T {
  real_T state_integrator_CSTATE[15];  /* '<Root>/state_integrator' */
};

/* State derivatives (default storage) */
struct XDot_skipper_lqi_T {
  real_T state_integrator_CSTATE[15];  /* '<Root>/state_integrator' */
};

/* State disabled  */
struct XDis_skipper_lqi_T {
  boolean_T state_integrator_CSTATE[15];/* '<Root>/state_integrator' */
};

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
struct ODE3_IntgData {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
};

#endif

/* Constant parameters (default storage) */
struct ConstP_skipper_lqi_T {
  /* Expression: Uff
   * Referenced by: '<Root>/ff_control'
   */
  real_T ff_control_Value[4];

  /* Expression: d
   * Referenced by: '<Root>/gravity'
   */
  real_T gravity_Value[12];

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

  /* Expression: [M*g*4 1 pi/22 pi/22]
   * Referenced by: '<S1>/saturator'
   */
  real_T saturator_UpperSat[4];

  /* Expression: [0 -1 -pi/22 -pi/22]
   * Referenced by: '<S1>/saturator'
   */
  real_T saturator_LowerSat[4];

  /* Expression: A
   * Referenced by: '<Root>/state_gain'
   */
  real_T state_gain_Gain[144];

  /* Expression: -E
   * Referenced by: '<Root>/extraction_gain'
   */
  real_T extraction_gain_Gain[36];

  /* Expression: B
   * Referenced by: '<Root>/input_gain'
   */
  real_T input_gain_Gain[48];
};

/* External outputs (root outports fed by signals with default storage) */
struct ExtY_skipper_lqi_T {
  real_T altitude;                     /* '<Root>/Out1' */
  real_T crossrange;                   /* '<Root>/Out2' */
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
struct tag_RTM_skipper_lqi_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_skipper_lqi_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_skipper_lqi_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[15];
  real_T odeF[3][15];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint16_T clockTick0;
    time_T stepSize0;
    uint16_T clockTick1;
    boolean_T firstInitCondFlag;
    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Constant parameters (default storage) */
extern const ConstP_skipper_lqi_T skipper_lqi_ConstP;

/* Class declaration for model skipper_lqi */
class skipper_lqi final
{
  /* public data and function members */
 public:
  /* Copy Constructor */
  skipper_lqi(skipper_lqi const&) = delete;

  /* Assignment Operator */
  skipper_lqi& operator= (skipper_lqi const&) & = delete;

  /* Move Constructor */
  skipper_lqi(skipper_lqi &&) = delete;

  /* Move Assignment Operator */
  skipper_lqi& operator= (skipper_lqi &&) = delete;

  /* Real-Time Model get method */
  RT_MODEL_skipper_lqi_T * getRTM();

  /* External outputs */
  ExtY_skipper_lqi_T skipper_lqi_Y;

  /* Initial conditions function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  static void terminate();

  /* Constructor */
  skipper_lqi();

  /* Destructor */
  ~skipper_lqi();

  /* private data and function members */
 private:
  /* Block signals */
  B_skipper_lqi_T skipper_lqi_B;

  /* Block states */
  DW_skipper_lqi_T skipper_lqi_DW;

  /* Block continuous states */
  X_skipper_lqi_T skipper_lqi_X;

  /* Block Continuous state disabled vector */
  XDis_skipper_lqi_T skipper_lqi_XDis;

  /* Global mass matrix */

  /* Continuous states update member function*/
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  /* Derivatives member function */
  void skipper_lqi_derivatives();

  /* Real-Time Model */
  RT_MODEL_skipper_lqi_T skipper_lqi_M;
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
 * '<Root>' : 'skipper_lqi'
 * '<S1>'   : 'skipper_lqi/control_constraints'
 */
#endif                                 /* skipper_lqi_h_ */
