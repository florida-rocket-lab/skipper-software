/*
 * skipper_lqi_private.h
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

#ifndef skipper_lqi_private_h_
#define skipper_lqi_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "skipper_lqi_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetFirstInitCond
#define rtmSetFirstInitCond(rtm, val)  ((rtm)->Timing.firstInitCondFlag = (val))
#endif

#ifndef rtmIsFirstInitCond
#define rtmIsFirstInitCond(rtm)        ((rtm)->Timing.firstInitCondFlag)
#endif

#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

/* private model entry point functions */
extern void skipper_lqi_derivatives();

#endif                                 /* skipper_lqi_private_h_ */
