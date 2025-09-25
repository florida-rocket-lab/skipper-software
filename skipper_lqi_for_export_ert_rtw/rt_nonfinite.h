//
// Sponsored License - for use in support of a program or activity
// sponsored by MathWorks.  Not for government, commercial or other
// non-sponsored organizational use.
//
// File: rt_nonfinite.h
//
// Code generated for Simulink model 'skipper_lqi_for_export'.
//
// Model version                  : 1.123
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Sep 25 15:38:50 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef rt_nonfinite_h_
#define rt_nonfinite_h_
#include "rtwtypes.h"
#ifdef __cplusplus

extern "C"
{

#endif

  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  extern boolean_T rtIsInf(real_T value);
  extern boolean_T rtIsInfF(real32_T value);
  extern boolean_T rtIsNaN(real_T value);
  extern boolean_T rtIsNaNF(real32_T value);

#ifdef __cplusplus

}                                      // extern "C"

#endif
#endif                                 // rt_nonfinite_h_

//
// File trailer for generated code.
//
// [EOF]
//
