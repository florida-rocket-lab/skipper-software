/*
 * skipper_lqi_for_export.h
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

/* Block signals (default storage) */
struct B_skipper_lqi_for_export_T {
  real_T UnitDelay[12];                /* '<Root>/Unit Delay' */
  real_T state_with_perturbation[12];  /* '<Root>/feedback_sum' */
  real_T state_gain[12];               /* '<Root>/state_gain' */
  real_T extraction_gain[3];           /* '<Root>/extraction_gain' */
  real_T UnitDelay1[12];               /* '<Root>/Unit Delay1' */
  real_T state_with_error_derivative[15];/* '<Root>/state_derivative_sum' */
  real_T initial_state[12];            /* '<Root>/initial_state' */
  real_T initial_error[3];             /* '<Root>/initial_error ' */
  real_T state_with_error[15];         /* '<Root>/Discrete Integrator' */
  real_T output_gain[12];              /* '<Root>/output_gain' */
  real_T fb_error_control[4];          /* '<Root>/lqi_gain' */
  real_T fb_state_control[4];          /* '<Root>/lqr_gain' */
  real_T net_control[4];               /* '<Root>/control_sum' */
  real_T saturator[4];                 /* '<S2>/saturator' */
  real_T upper_gimbal_angle;           /* '<S2>/Rate Limiter1' */
  real_T lower_gimbal_angle;           /* '<S2>/Rate Limiter' */
  real_T passthrough_gain[12];         /* '<Root>/passthrough_gain' */
  real_T output[12];                   /* '<Root>/Sum2' */
  real_T MemoryX[12];                  /* '<S1>/MemoryX' */
  real_T Akxhatkk1[12];                /* '<S23>/A[k]*xhat[k|k-1]' */
  real_T Reshapeu[4];                  /* '<S1>/Reshapeu' */
  real_T Bkuk[12];                     /* '<S23>/B[k]*u[k]' */
  real_T Add[12];                      /* '<S23>/Add' */
  real_T Add_j[12];                    /* '<S30>/Add' */
  real_T input_gain[12];               /* '<Root>/input_gain' */
  real_T Product[12];                  /* '<S56>/Product' */
  real_T Add1[12];                     /* '<S56>/Add1' */
  real_T Product2[12];                 /* '<S56>/Product2' */
  real_T Ckxhatkk1[12];                /* '<S54>/C[k]*xhat[k|k-1]' */
  real_T Dkuk[12];                     /* '<S54>/D[k]*u[k]' */
  real_T yhatkk1[12];                  /* '<S54>/Add1' */
  real_T Sum[12];                      /* '<S54>/Sum' */
  real_T Product3[12];                 /* '<S54>/Product3' */
  real_T P[144];                       /* '<S52>/SqrtUsedFcn' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_skipper_lqi_for_export_T {
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
  real_T saturator_UpperSat[4];        /* Expression: [T_max0 1 pi/22 pi/22]'
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
};

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

  /* model start function */
  void start();

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
  /* Block signals */
  B_skipper_lqi_for_export_T skipper_lqi_for_export_B;

  /* Block states */
  DW_skipper_lqi_for_export_T skipper_lqi_for_export_DW;

  /* Tunable parameters */
  static P_skipper_lqi_for_export_T skipper_lqi_for_export_P;

  /* Real-Time Model */
  RT_MODEL_skipper_lqi_for_expo_T skipper_lqi_for_export_M;
};

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S46>/Data Type Duplicate' : Unused code path elimination
 * Block '<S47>/Data Type Duplicate' : Unused code path elimination
 * Block '<S48>/Conversion' : Unused code path elimination
 * Block '<S48>/Data Type Duplicate' : Unused code path elimination
 * Block '<S49>/Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/Data Type Duplicate' : Unused code path elimination
 * Block '<S7>/Data Type Duplicate' : Unused code path elimination
 * Block '<S8>/Data Type Duplicate' : Unused code path elimination
 * Block '<S9>/Data Type Duplicate' : Unused code path elimination
 * Block '<S11>/Data Type Duplicate' : Unused code path elimination
 * Block '<S12>/Data Type Duplicate' : Unused code path elimination
 * Block '<S13>/Data Type Duplicate' : Unused code path elimination
 * Block '<S14>/Conversion' : Unused code path elimination
 * Block '<S14>/Data Type Duplicate' : Unused code path elimination
 * Block '<S15>/Data Type Duplicate' : Unused code path elimination
 * Block '<S16>/Data Type Duplicate' : Unused code path elimination
 * Block '<S17>/Data Type Duplicate' : Unused code path elimination
 * Block '<S19>/Data Type Duplicate' : Unused code path elimination
 * Block '<S20>/Data Type Duplicate' : Unused code path elimination
 * Block '<S1>/G' : Unused code path elimination
 * Block '<S1>/H' : Unused code path elimination
 * Block '<S1>/N' : Unused code path elimination
 * Block '<S1>/P0' : Unused code path elimination
 * Block '<S1>/Q' : Unused code path elimination
 * Block '<S1>/R' : Unused code path elimination
 * Block '<S44>/CheckSignalProperties' : Unused code path elimination
 * Block '<S45>/CheckSignalProperties' : Unused code path elimination
 * Block '<S46>/Conversion' : Eliminate redundant data type conversion
 * Block '<S47>/Conversion' : Eliminate redundant data type conversion
 * Block '<S49>/Conversion' : Eliminate redundant data type conversion
 * Block '<S19>/Conversion' : Eliminate redundant data type conversion
 * Block '<S23>/Reshape' : Reshape block reduction
 * Block '<S1>/ReshapeX0' : Reshape block reduction
 * Block '<S1>/Reshapexhat' : Reshape block reduction
 * Block '<S1>/Reshapey' : Reshape block reduction
 */

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
