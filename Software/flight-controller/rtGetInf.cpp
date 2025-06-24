/*
 * rtGetInf.cpp
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

#include "rtwtypes.h"

extern "C"
{

#include "rtGetInf.h"

}

extern "C"
{
  /* Return rtInf needed by the generated code. */
  real_T rtGetInf(void)
  {
    return rtInf;
  }

  /* Get rtInfF needed by the generated code. */
  real32_T rtGetInfF(void)
  {
    return rtInfF;
  }

  /* Return rtMinusInf needed by the generated code. */
  real_T rtGetMinusInf(void)
  {
    return rtMinusInf;
  }

  /* Return rtMinusInfF needed by the generated code. */
  real32_T rtGetMinusInfF(void)
  {
    return rtMinusInfF;
  }
}
