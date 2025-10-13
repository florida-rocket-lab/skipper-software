/*
 * skipper_lqi_for_export.h
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

#ifndef skipper_lqi_for_export_h_
#define skipper_lqi_for_export_h_
#ifndef skipper_lqi_for_export_COMMON_INCLUDES_
#define skipper_lqi_for_export_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                             /* skipper_lqi_for_export_COMMON_INCLUDES_ */

#include "skipper_lqi_for_export_types.h"
#include <float.h>
#include <string.h>
#include <stddef.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
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
#define rtmGetT(rtm)                   ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&(rtm)->Timing.taskTime0)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T initial_state[12];            /* '<Root>/initial_state' */
  real_T initial_error[3];             /* '<Root>/initial_error ' */
  real_T Product2[12];                 /* '<S56>/Product2' */
  real_T Product3[12];                 /* '<S54>/Product3' */
} B_skipper_lqi_for_export_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE[12];         /* '<Root>/Unit Delay' */
  real_T UnitDelay1_DSTATE[12];        /* '<Root>/Unit Delay1' */
  real_T DiscreteIntegrator_DSTATE[15];/* '<Root>/Discrete Integrator' */
  real_T MemoryX_DSTATE[12];           /* '<S1>/MemoryX' */
  real_T PrevY;                        /* '<S2>/Rate Limiter3' */
  real_T PrevY_k;                      /* '<S2>/Rate Limiter2' */
  real_T PrevY_b;                      /* '<S2>/Rate Limiter1' */
  real_T PrevY_l;                      /* '<S2>/Rate Limiter' */
  boolean_T icLoad;                    /* '<S1>/MemoryX' */
  boolean_T EnabledSubsystem_MODE;     /* '<S30>/Enabled Subsystem' */
  boolean_T MeasurementUpdate_MODE;    /* '<S23>/MeasurementUpdate' */
} DW_skipper_lqi_for_export_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T imu_state_in[12];             /* '<Root>/imu_state_in' */
  real_T reference[3];                 /* '<Root>/refeference_in' */
} ExtU_skipper_lqi_for_export_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
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
} ExtY_skipper_lqi_for_export_T;

/* Parameters (default storage) */
struct P_skipper_lqi_for_export_T_ {
  real_T A[144];                       /* Variable: A
                                        * Referenced by: '<Root>/state_gain'
                                        */
  real_T B[48];                        /* Variable: B
                                        * Referenced by: '<Root>/input_gain'
                                        */
  real_T C[144];                       /* Variable: C
                                        * Referenced by: '<Root>/output_gain'
                                        */
  real_T D[48];                        /* Variable: D
                                        * Referenced by: '<Root>/passthrough_gain'
                                        */
  real_T E[36];                        /* Variable: E
                                        * Referenced by: '<Root>/extraction_gain'
                                        */
  real_T K1[48];                       /* Variable: K1
                                        * Referenced by: '<Root>/lqr_gain'
                                        */
  real_T K2[12];                       /* Variable: K2
                                        * Referenced by: '<Root>/lqi_gain'
                                        */
  real_T Uff[4];                       /* Variable: Uff
                                        * Referenced by: '<Root>/ff_control'
                                        */
  real_T d[12];                        /* Variable: d
                                        * Referenced by: '<Root>/gravity_const'
                                        */
  real_T Lykyhatkk1_Y0;                /* Expression: 0
                                        * Referenced by: '<S54>/L*(y[k]-yhat[k|k-1])'
                                        */
  real_T deltax_Y0;                    /* Expression: 0
                                        * Referenced by: '<S56>/deltax'
                                        */
  real_T perturbation_Value[12];       /* Expression: [0 0 0 0 0 0 0 0 0 0 0 0]'
                                        * Referenced by: '<Root>/perturbation'
                                        */
  real_T UnitDelay_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<Root>/Unit Delay'
                                        */
  real_T UnitDelay1_InitialCondition;  /* Expression: 0
                                        * Referenced by: '<Root>/Unit Delay1'
                                        */
  real_T dummy_zeros_Value[3];         /* Expression: [0 0 0]'
                                        * Referenced by: '<Root>/dummy_zeros'
                                        */
  real_T initial_state_Value[12];      /* Expression: [0 0 0 0 0 0 0 0 0 0 0 0]'
                                        * Referenced by: '<Root>/initial_state'
                                        */
  real_T initial_error_Value[3];       /* Expression: [0 0 0]'
                                        * Referenced by: '<Root>/initial_error '
                                        */
  real_T DiscreteIntegrator_gainval;
                               /* Computed Parameter: DiscreteIntegrator_gainval
                                * Referenced by: '<Root>/Discrete Integrator'
                                */
  real_T saturator_UpperSat[4];        /* Expression: [3.5 1 pi/22 pi/22]'
                                        * Referenced by: '<S2>/saturator'
                                        */
  real_T saturator_LowerSat[4];        /* Expression: [0 -1 -pi/22 -pi/22]'
                                        * Referenced by: '<S2>/saturator'
                                        */
  real_T RateLimiter3_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S2>/Rate Limiter3'
                                        */
  real_T RateLimiter3_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S2>/Rate Limiter3'
                                        */
  real_T RateLimiter3_IC;              /* Expression: 0
                                        * Referenced by: '<S2>/Rate Limiter3'
                                        */
  real_T RateLimiter2_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S2>/Rate Limiter2'
                                        */
  real_T RateLimiter2_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S2>/Rate Limiter2'
                                        */
  real_T RateLimiter2_IC;              /* Expression: 1.7
                                        * Referenced by: '<S2>/Rate Limiter2'
                                        */
  real_T RateLimiter1_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S2>/Rate Limiter1'
                                        */
  real_T RateLimiter1_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S2>/Rate Limiter1'
                                        */
  real_T RateLimiter1_IC;              /* Expression: 1.7
                                        * Referenced by: '<S2>/Rate Limiter1'
                                        */
  real_T RateLimiter_RisingLim;        /* Expression: inf
                                        * Referenced by: '<S2>/Rate Limiter'
                                        */
  real_T RateLimiter_FallingLim;       /* Expression: -inf
                                        * Referenced by: '<S2>/Rate Limiter'
                                        */
  real_T RateLimiter_IC;               /* Expression: 1.7
                                        * Referenced by: '<S2>/Rate Limiter'
                                        */
  real_T Gain12_Gain;                  /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain12'
                                        */
  real_T Gain11_Gain;                  /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain11'
                                        */
  real_T Gain10_Gain;                  /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain10'
                                        */
  real_T Gain9_Gain;                   /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain9'
                                        */
  real_T Gain8_Gain;                   /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain8'
                                        */
  real_T Gain7_Gain;                   /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain7'
                                        */
  real_T Gain5_Gain;                   /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain5'
                                        */
  real_T Gain6_Gain;                   /* Expression: 180/pi
                                        * Referenced by: '<Root>/Gain6'
                                        */
  real_T A_Value[144];                 /* Expression: pInitialization.A
                                        * Referenced by: '<S1>/A'
                                        */
  real_T B_Value[48];                  /* Expression: pInitialization.B
                                        * Referenced by: '<S1>/B'
                                        */
  real_T C_Value[144];                 /* Expression: pInitialization.C
                                        * Referenced by: '<S1>/C'
                                        */
  real_T CovarianceZ_Value[144];       /* Expression: pInitialization.Z
                                        * Referenced by: '<S3>/CovarianceZ'
                                        */
  real_T KalmanGainL_Value[144];       /* Expression: pInitialization.L
                                        * Referenced by: '<S3>/KalmanGainL'
                                        */
  real_T KalmanGainM_Value[144];       /* Expression: pInitialization.M
                                        * Referenced by: '<S3>/KalmanGainM'
                                        */
  real_T D_Value[48];                  /* Expression: pInitialization.D
                                        * Referenced by: '<S1>/D'
                                        */
  real_T X0_Value[12];                 /* Expression: pInitialization.X0
                                        * Referenced by: '<S1>/X0'
                                        */
  boolean_T isSqrtUsed_Value;          /* Expression: pInitialization.isSqrtUsed
                                        * Referenced by: '<S52>/isSqrtUsed'
                                        */
  boolean_T Enable_Value;              /* Expression: true()
                                        * Referenced by: '<S1>/Enable'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_skipper_lqi_for_expor_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (default storage) */
extern P_skipper_lqi_for_export_T skipper_lqi_for_export_P;

/* Block signals (default storage) */
extern B_skipper_lqi_for_export_T skipper_lqi_for_export_B;

/* Block states (default storage) */
extern DW_skipper_lqi_for_export_T skipper_lqi_for_export_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_skipper_lqi_for_export_T skipper_lqi_for_export_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_skipper_lqi_for_export_T skipper_lqi_for_export_Y;

/* Model entry point functions */
extern void skipper_lqi_for_export_initialize(void);
extern void skipper_lqi_for_export_step(void);
extern void skipper_lqi_for_export_terminate(void);

/* Real-time Model object */
extern RT_MODEL_skipper_lqi_for_expo_T *const skipper_lqi_for_export_M;

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
 * '<S1>'   : 'skipper_lqi_for_export/Kalman Filter'
 * '<S2>'   : 'skipper_lqi_for_export/control_constraints'
 * '<S3>'   : 'skipper_lqi_for_export/Kalman Filter/CalculatePL'
 * '<S4>'   : 'skipper_lqi_for_export/Kalman Filter/CalculateYhat'
 * '<S5>'   : 'skipper_lqi_for_export/Kalman Filter/CovarianceOutputConfigurator'
 * '<S6>'   : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionA'
 * '<S7>'   : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionB'
 * '<S8>'   : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionC'
 * '<S9>'   : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionD'
 * '<S10>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionEnable'
 * '<S11>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionG'
 * '<S12>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionH'
 * '<S13>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionN'
 * '<S14>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionP'
 * '<S15>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionP0'
 * '<S16>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionQ'
 * '<S17>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionR'
 * '<S18>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionReset'
 * '<S19>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionX'
 * '<S20>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionX0'
 * '<S21>'  : 'skipper_lqi_for_export/Kalman Filter/DataTypeConversionu'
 * '<S22>'  : 'skipper_lqi_for_export/Kalman Filter/MemoryP'
 * '<S23>'  : 'skipper_lqi_for_export/Kalman Filter/Observer'
 * '<S24>'  : 'skipper_lqi_for_export/Kalman Filter/ReducedQRN'
 * '<S25>'  : 'skipper_lqi_for_export/Kalman Filter/Reset'
 * '<S26>'  : 'skipper_lqi_for_export/Kalman Filter/Reshapeyhat'
 * '<S27>'  : 'skipper_lqi_for_export/Kalman Filter/ScalarExpansionP0'
 * '<S28>'  : 'skipper_lqi_for_export/Kalman Filter/ScalarExpansionQ'
 * '<S29>'  : 'skipper_lqi_for_export/Kalman Filter/ScalarExpansionR'
 * '<S30>'  : 'skipper_lqi_for_export/Kalman Filter/UseCurrentEstimator'
 * '<S31>'  : 'skipper_lqi_for_export/Kalman Filter/checkA'
 * '<S32>'  : 'skipper_lqi_for_export/Kalman Filter/checkB'
 * '<S33>'  : 'skipper_lqi_for_export/Kalman Filter/checkC'
 * '<S34>'  : 'skipper_lqi_for_export/Kalman Filter/checkD'
 * '<S35>'  : 'skipper_lqi_for_export/Kalman Filter/checkEnable'
 * '<S36>'  : 'skipper_lqi_for_export/Kalman Filter/checkG'
 * '<S37>'  : 'skipper_lqi_for_export/Kalman Filter/checkH'
 * '<S38>'  : 'skipper_lqi_for_export/Kalman Filter/checkN'
 * '<S39>'  : 'skipper_lqi_for_export/Kalman Filter/checkP0'
 * '<S40>'  : 'skipper_lqi_for_export/Kalman Filter/checkQ'
 * '<S41>'  : 'skipper_lqi_for_export/Kalman Filter/checkR'
 * '<S42>'  : 'skipper_lqi_for_export/Kalman Filter/checkReset'
 * '<S43>'  : 'skipper_lqi_for_export/Kalman Filter/checkX0'
 * '<S44>'  : 'skipper_lqi_for_export/Kalman Filter/checku'
 * '<S45>'  : 'skipper_lqi_for_export/Kalman Filter/checky'
 * '<S46>'  : 'skipper_lqi_for_export/Kalman Filter/CalculatePL/DataTypeConversionL'
 * '<S47>'  : 'skipper_lqi_for_export/Kalman Filter/CalculatePL/DataTypeConversionM'
 * '<S48>'  : 'skipper_lqi_for_export/Kalman Filter/CalculatePL/DataTypeConversionP'
 * '<S49>'  : 'skipper_lqi_for_export/Kalman Filter/CalculatePL/DataTypeConversionZ'
 * '<S50>'  : 'skipper_lqi_for_export/Kalman Filter/CalculatePL/Ground'
 * '<S51>'  : 'skipper_lqi_for_export/Kalman Filter/CalculateYhat/Ground'
 * '<S52>'  : 'skipper_lqi_for_export/Kalman Filter/CovarianceOutputConfigurator/decideOutput'
 * '<S53>'  : 'skipper_lqi_for_export/Kalman Filter/CovarianceOutputConfigurator/decideOutput/SqrtUsedFcn'
 * '<S54>'  : 'skipper_lqi_for_export/Kalman Filter/Observer/MeasurementUpdate'
 * '<S55>'  : 'skipper_lqi_for_export/Kalman Filter/ReducedQRN/Ground'
 * '<S56>'  : 'skipper_lqi_for_export/Kalman Filter/UseCurrentEstimator/Enabled Subsystem'
 */
#endif                                 /* skipper_lqi_for_export_h_ */
