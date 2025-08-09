/*
 * rt_nonfinite.h
 *
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 *
 * Code generation for model "skipper_lqi_for_export".
 *
 * Model version              : 1.122
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C++ source code generated on : Sat Aug  9 19:26:02 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Debugging
 * Validation result: Not run
 */

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

}                                      /* extern "C" */

#endif
#endif                                 /* rt_nonfinite_h_ */
