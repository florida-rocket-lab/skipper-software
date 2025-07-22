#ifndef TMWTYPES_H_
#define TMWTYPES_H_

#include <stdint.h>
#include <stdbool.h>

/* NEW ― required by Simulink headers */
typedef char      char_T;     // <-- add
typedef unsigned char uchar_T;

/* Existing aliases … */
typedef int8_t    int8_T;
typedef uint8_t   uint8_T;
typedef int16_t   int16_T;
typedef uint16_t  uint16_T;
typedef int32_t   int32_T;
typedef uint32_t  uint32_T;
typedef float     real32_T;
typedef double    real64_T;
typedef real32_T  real_T;
typedef uint8_T   boolean_T;
typedef int32_T   int_T;
typedef uint32_T  uint_T;

#endif /* TMWTYPES_H_ */
