#ifndef __TMWTYPES_H__
#define __TMWTYPES_H__

/*
 * tmwtypes.h
 * Minimal Simulink R2018+ style type definitions
 */

#include <stddef.h>
#include <stdint.h>

/* Boolean */
typedef uint8_t           boolean_T;

/* Character */
typedef char              char_T;
typedef uint8_t           uchar_T;

/* Integer types */
typedef int8_t            int8_T;
typedef uint8_t           uint8_T;
typedef int16_t           int16_T;
typedef uint16_t          uint16_T;
typedef int32_t           int32_T;
typedef uint32_t          uint32_T;
typedef int64_t           int64_T;
typedef uint64_t          uint64_T;

/* Real (floating-point) */
typedef float             real32_T;
typedef double            real_T;

/* Generic integer */
typedef int32_t           int_T;
typedef uint32_t          uint_T;

/* Min/max macros */
#ifndef TMWTYPES_PREVIOUSLY_INCLUDED
# define false           (0U)
# define true            (1U)
#endif

#endif /* __TMWTYPES_H__ */
