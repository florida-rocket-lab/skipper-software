//
// Sponsored License - for use in support of a program or activity
// sponsored by MathWorks.  Not for government, commercial or other
// non-sponsored organizational use.
//
// File: skipper_lqi_for_export.h
//
// Code generated for Simulink model 'skipper_lqi_for_export'.
//
// Model version                  : 1.123
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Sep 25 17:17:21 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef skipper_lqi_for_export_h_
#define skipper_lqi_for_export_h_
#include <cmath>
#include "rtwtypes.h"

// Class declaration for model skipper_lqi_for_export
class skipperLQR final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW {
    real_T UnitDelay_DSTATE[12];       // '<Root>/Unit Delay'
    real_T UnitDelay1_DSTATE[12];      // '<Root>/Unit Delay1'
    real_T DiscreteIntegrator_DSTATE[15];// '<Root>/Discrete Integrator'
    real_T PrevY;                      // '<S1>/Rate Limiter3'
    real_T PrevY_k;                    // '<S1>/Rate Limiter2'
    real_T PrevY_b;                    // '<S1>/Rate Limiter1'
    real_T PrevY_l;                    // '<S1>/Rate Limiter'
  };

  // Invariant block signals (default storage)
  struct ConstB {
    real_T initial_error[3];           // '<Root>/initial_error'
    real_T initial_state[12];          // '<Root>/initial_state'
  };

  // Constant parameters (default storage)
  struct ConstP {
    // Expression: d
    //  Referenced by: '<Root>/gravity_const'

    real_T gravity_const_Value[12];

    // Expression: A
    //  Referenced by: '<Root>/state_gain'

    real_T state_gain_Gain[144];

    // Expression: -E
    //  Referenced by: '<Root>/extraction_gain'

    real_T extraction_gain_Gain[36];

    // Expression: C
    //  Referenced by: '<Root>/output_gain'

    real_T output_gain_Gain[144];

    // Expression: -K2
    //  Referenced by: '<Root>/lqi_gain'

    real_T lqi_gain_Gain[12];

    // Expression: -K1
    //  Referenced by: '<Root>/lqr_gain'

    real_T lqr_gain_Gain[48];

    // Expression: B
    //  Referenced by: '<Root>/input_gain'

    real_T input_gain_Gain[48];
  };

  // External inputs (root inport signals with default storage)
  struct ExtU {
    real_T imu_state_in[12];           // '<Root>/imu_state_in'
    real_T reference[3];               // '<Root>/refeference_in'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY {
    real_T altitude;                   // '<Root>/Out1'
    real_T crossrange;                 // '<Root>/Out2'
    real_T control[4];                 // '<Root>/control_commands_out'
    real_T downrange;                  // '<Root>/Out3'
    real_T lateral_vel;                // '<Root>/Out4'
    real_T longitudinal_vel;           // '<Root>/Out5'
    real_T directional_vel;            // '<Root>/Out6'
    real_T roll;                       // '<Root>/Out7'
    real_T pitch;                      // '<Root>/Out8'
    real_T yaw;                        // '<Root>/Out9'
    real_T lateral_rate;               // '<Root>/Out10'
    real_T longitudinal_rate;          // '<Root>/Out11'
    real_T directional_rate;           // '<Root>/Out12'
    real_T thrust;                     // '<Root>/Out13'
    real_T reaction_torque;            // '<Root>/Out14'
    real_T upper_gimbal_angle;         // '<Root>/Out15'
    real_T lower_gimbal_angle;         // '<Root>/Out16'
    real_T altitude_error;             // '<Root>/Out17'
    real_T crossrange_error;           // '<Root>/Out18'
    real_T downrange_error;            // '<Root>/Out19'
  };

  // Copy Constructor
  skipperLQR(skipperLQR const&) = delete;

  // Assignment Operator
  skipperLQR& operator= (skipperLQR const&) & = delete;

  // Move Constructor
  skipperLQR(skipperLQR &&) = delete;

  // Move Assignment Operator
  skipperLQR& operator= (skipperLQR &&) = delete;

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  skipperLQR();

  // Destructor
  ~skipperLQR();

  // private data and function members
 private:
  // Block states
  DW rtDW;
};

extern const skipperLQR::ConstB rtConstB;// constant block i/o

// Constant parameters (default storage)
extern const skipperLQR::ConstP rtConstP;
extern "C"
{
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  static boolean_T rtIsInf(real_T value);
  static boolean_T rtIsInfF(real32_T value);
  static boolean_T rtIsNaN(real_T value);
  static boolean_T rtIsNaNF(real32_T value);
}                                      // extern "C"

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'skipper_lqi_for_export'
//  '<S1>'   : 'skipper_lqi_for_export/control_constraints'

#endif                                 // skipper_lqi_for_export_h_

//
// File trailer for generated code.
//
// [EOF]
//
