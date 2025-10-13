/*
 * skipper_lqi.h
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

#ifndef skipper_lqi_h_
#define skipper_lqi_h_
#include <cmath>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "skipper_lqi_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
struct B_skipper_lqi_T {
  real_T initial_state[12];            /* '<Root>/initial_state' */
  real_T initial_error[3];             /* '<Root>/initial_error' */
  real_T Output[12];                   /* '<S1>/Output' */
  real_T Product2[12];                 /* '<S57>/Product2' */
  real_T Product3[12];                 /* '<S55>/Product3' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_skipper_lqi_T {
  real_T UnitDelay_DSTATE[12];         /* '<Root>/Unit Delay' */
  real_T UnitDelay1_DSTATE[12];        /* '<Root>/Unit Delay1' */
  real_T DiscreteIntegrator_DSTATE[15];/* '<Root>/Discrete Integrator' */
  real_T MemoryX_DSTATE[12];           /* '<S2>/MemoryX' */
  real_T PrevY;                        /* '<S3>/Rate Limiter3' */
  real_T PrevY_k;                      /* '<S3>/Rate Limiter2' */
  real_T PrevY_b;                      /* '<S3>/Rate Limiter1' */
  real_T PrevY_l;                      /* '<S3>/Rate Limiter' */
  real_T NextOutput;                   /* '<S1>/White Noise' */
  uint32_T RandSeed;                   /* '<S1>/White Noise' */
  boolean_T icLoad;                    /* '<S2>/MemoryX' */
  boolean_T EnabledSubsystem_MODE;     /* '<S31>/Enabled Subsystem' */
  boolean_T MeasurementUpdate_MODE;    /* '<S24>/MeasurementUpdate' */
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

/* Parameters (default storage) */
struct P_skipper_lqi_T_ {
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
  real_T BandLimitedWhiteNoise_Cov[12];
                                    /* Mask Parameter: BandLimitedWhiteNoise_Cov
                                     * Referenced by: '<S1>/Output'
                                     */
  real_T BandLimitedWhiteNoise_seed;
                                   /* Mask Parameter: BandLimitedWhiteNoise_seed
                                    * Referenced by: '<S1>/White Noise'
                                    */
  real_T Lykyhatkk1_Y0;                /* Expression: 0
                                        * Referenced by: '<S55>/L*(y[k]-yhat[k|k-1])'
                                        */
  real_T deltax_Y0;                    /* Expression: 0
                                        * Referenced by: '<S57>/deltax'
                                        */
  real_T reference_const_Value[3];     /* Expression: [x_ref y_ref z_ref]'
                                        * Referenced by: '<Root>/reference_const'
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
                                        * Referenced by: '<Root>/initial_error'
                                        */
  real_T DiscreteIntegrator_gainval;
                               /* Computed Parameter: DiscreteIntegrator_gainval
                                * Referenced by: '<Root>/Discrete Integrator'
                                */
  real_T saturator_UpperSat[4];        /* Expression: [T_max 1 pi/22 pi/22]
                                        * Referenced by: '<S3>/saturator'
                                        */
  real_T saturator_LowerSat[4];        /* Expression: [0 -1 -pi/22 -pi/22]
                                        * Referenced by: '<S3>/saturator'
                                        */
  real_T RateLimiter3_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S3>/Rate Limiter3'
                                        */
  real_T RateLimiter3_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S3>/Rate Limiter3'
                                        */
  real_T RateLimiter3_IC;              /* Expression: 0
                                        * Referenced by: '<S3>/Rate Limiter3'
                                        */
  real_T RateLimiter2_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S3>/Rate Limiter2'
                                        */
  real_T RateLimiter2_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S3>/Rate Limiter2'
                                        */
  real_T RateLimiter2_IC;              /* Expression: 1.7
                                        * Referenced by: '<S3>/Rate Limiter2'
                                        */
  real_T RateLimiter1_RisingLim;       /* Expression: inf
                                        * Referenced by: '<S3>/Rate Limiter1'
                                        */
  real_T RateLimiter1_FallingLim;      /* Expression: -inf
                                        * Referenced by: '<S3>/Rate Limiter1'
                                        */
  real_T RateLimiter1_IC;              /* Expression: 1.7
                                        * Referenced by: '<S3>/Rate Limiter1'
                                        */
  real_T RateLimiter_RisingLim;        /* Expression: inf
                                        * Referenced by: '<S3>/Rate Limiter'
                                        */
  real_T RateLimiter_FallingLim;       /* Expression: -inf
                                        * Referenced by: '<S3>/Rate Limiter'
                                        */
  real_T RateLimiter_IC;               /* Expression: 1.7
                                        * Referenced by: '<S3>/Rate Limiter'
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
  real_T WhiteNoise_Mean;              /* Expression: 0
                                        * Referenced by: '<S1>/White Noise'
                                        */
  real_T WhiteNoise_StdDev;            /* Computed Parameter: WhiteNoise_StdDev
                                        * Referenced by: '<S1>/White Noise'
                                        */
  real_T A_Value[144];                 /* Expression: pInitialization.A
                                        * Referenced by: '<S2>/A'
                                        */
  real_T B_Value[48];                  /* Expression: pInitialization.B
                                        * Referenced by: '<S2>/B'
                                        */
  real_T C_Value[144];                 /* Expression: pInitialization.C
                                        * Referenced by: '<S2>/C'
                                        */
  real_T CovarianceZ_Value[144];       /* Expression: pInitialization.Z
                                        * Referenced by: '<S4>/CovarianceZ'
                                        */
  real_T KalmanGainL_Value[144];       /* Expression: pInitialization.L
                                        * Referenced by: '<S4>/KalmanGainL'
                                        */
  real_T KalmanGainM_Value[144];       /* Expression: pInitialization.M
                                        * Referenced by: '<S4>/KalmanGainM'
                                        */
  real_T D_Value[48];                  /* Expression: pInitialization.D
                                        * Referenced by: '<S2>/D'
                                        */
  real_T X0_Value[12];                 /* Expression: pInitialization.X0
                                        * Referenced by: '<S2>/X0'
                                        */
  boolean_T isSqrtUsed_Value;          /* Expression: pInitialization.isSqrtUsed
                                        * Referenced by: '<S53>/isSqrtUsed'
                                        */
  boolean_T Enable_Value;              /* Expression: true()
                                        * Referenced by: '<S2>/Enable'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_skipper_lqi_T {
  const char_T *errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

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

  /* Root outports get method */
  const ExtY_skipper_lqi_T &getExternalOutputs() const
  {
    return skipper_lqi_Y;
  }

  /* model start function */
  void start();

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
  /* External outputs */
  ExtY_skipper_lqi_T skipper_lqi_Y;

  /* Block signals */
  B_skipper_lqi_T skipper_lqi_B;

  /* Block states */
  DW_skipper_lqi_T skipper_lqi_DW;

  /* Tunable parameters */
  static P_skipper_lqi_T skipper_lqi_P;

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
 * '<S1>'   : 'skipper_lqi/Band-Limited White Noise'
 * '<S2>'   : 'skipper_lqi/Kalman Filter'
 * '<S3>'   : 'skipper_lqi/control_constraints'
 * '<S4>'   : 'skipper_lqi/Kalman Filter/CalculatePL'
 * '<S5>'   : 'skipper_lqi/Kalman Filter/CalculateYhat'
 * '<S6>'   : 'skipper_lqi/Kalman Filter/CovarianceOutputConfigurator'
 * '<S7>'   : 'skipper_lqi/Kalman Filter/DataTypeConversionA'
 * '<S8>'   : 'skipper_lqi/Kalman Filter/DataTypeConversionB'
 * '<S9>'   : 'skipper_lqi/Kalman Filter/DataTypeConversionC'
 * '<S10>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionD'
 * '<S11>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionEnable'
 * '<S12>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionG'
 * '<S13>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionH'
 * '<S14>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionN'
 * '<S15>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionP'
 * '<S16>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionP0'
 * '<S17>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionQ'
 * '<S18>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionR'
 * '<S19>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionReset'
 * '<S20>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionX'
 * '<S21>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionX0'
 * '<S22>'  : 'skipper_lqi/Kalman Filter/DataTypeConversionu'
 * '<S23>'  : 'skipper_lqi/Kalman Filter/MemoryP'
 * '<S24>'  : 'skipper_lqi/Kalman Filter/Observer'
 * '<S25>'  : 'skipper_lqi/Kalman Filter/ReducedQRN'
 * '<S26>'  : 'skipper_lqi/Kalman Filter/Reset'
 * '<S27>'  : 'skipper_lqi/Kalman Filter/Reshapeyhat'
 * '<S28>'  : 'skipper_lqi/Kalman Filter/ScalarExpansionP0'
 * '<S29>'  : 'skipper_lqi/Kalman Filter/ScalarExpansionQ'
 * '<S30>'  : 'skipper_lqi/Kalman Filter/ScalarExpansionR'
 * '<S31>'  : 'skipper_lqi/Kalman Filter/UseCurrentEstimator'
 * '<S32>'  : 'skipper_lqi/Kalman Filter/checkA'
 * '<S33>'  : 'skipper_lqi/Kalman Filter/checkB'
 * '<S34>'  : 'skipper_lqi/Kalman Filter/checkC'
 * '<S35>'  : 'skipper_lqi/Kalman Filter/checkD'
 * '<S36>'  : 'skipper_lqi/Kalman Filter/checkEnable'
 * '<S37>'  : 'skipper_lqi/Kalman Filter/checkG'
 * '<S38>'  : 'skipper_lqi/Kalman Filter/checkH'
 * '<S39>'  : 'skipper_lqi/Kalman Filter/checkN'
 * '<S40>'  : 'skipper_lqi/Kalman Filter/checkP0'
 * '<S41>'  : 'skipper_lqi/Kalman Filter/checkQ'
 * '<S42>'  : 'skipper_lqi/Kalman Filter/checkR'
 * '<S43>'  : 'skipper_lqi/Kalman Filter/checkReset'
 * '<S44>'  : 'skipper_lqi/Kalman Filter/checkX0'
 * '<S45>'  : 'skipper_lqi/Kalman Filter/checku'
 * '<S46>'  : 'skipper_lqi/Kalman Filter/checky'
 * '<S47>'  : 'skipper_lqi/Kalman Filter/CalculatePL/DataTypeConversionL'
 * '<S48>'  : 'skipper_lqi/Kalman Filter/CalculatePL/DataTypeConversionM'
 * '<S49>'  : 'skipper_lqi/Kalman Filter/CalculatePL/DataTypeConversionP'
 * '<S50>'  : 'skipper_lqi/Kalman Filter/CalculatePL/DataTypeConversionZ'
 * '<S51>'  : 'skipper_lqi/Kalman Filter/CalculatePL/Ground'
 * '<S52>'  : 'skipper_lqi/Kalman Filter/CalculateYhat/Ground'
 * '<S53>'  : 'skipper_lqi/Kalman Filter/CovarianceOutputConfigurator/decideOutput'
 * '<S54>'  : 'skipper_lqi/Kalman Filter/CovarianceOutputConfigurator/decideOutput/SqrtUsedFcn'
 * '<S55>'  : 'skipper_lqi/Kalman Filter/Observer/MeasurementUpdate'
 * '<S56>'  : 'skipper_lqi/Kalman Filter/ReducedQRN/Ground'
 * '<S57>'  : 'skipper_lqi/Kalman Filter/UseCurrentEstimator/Enabled Subsystem'
 */
#endif                                 /* skipper_lqi_h_ */
