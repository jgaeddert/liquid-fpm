// 
//
//

#ifndef __LIQUID_FPM_INTERNAL_H__
#define __LIQUID_FPM_INTERNAL_H__

#include "fpm.h"

#define FRACBITS 28

//
// Trigonometric helper functions, arrays, constants
//
#define FPM_Q32_2_PI       0x7FFFFFFF  ///< \f$2\pi\f$
#define FPM_Q32_PI         0x3FFFFFFF  ///< \f$\pi\f$
#define FPM_Q32_PI_BY_2    0x1FFFFFFF  ///< \f$\pi/2\f$
#define FPM_Q32_PI_BY_4    0x0FFFFFFF  ///< \f$\pi/4\f$

#define FPM_Q32_1_BY_SQRT_2 0x0B504F33 ///< \f$ 1/sqrt(2) \f$

//#define sigprocc_angle_fixed_to_double(x) (mad_f_todouble(x) / mad_f_todouble(FPM_Q32_PI) * M_PI)

extern const q32_t sin_table_q32_256[256];

// fractional portion, \f$ f \f$ of log2, \f$ log2(x) = b + f/256 \f$
extern const unsigned int log2_fraction_table_256[256];
extern const unsigned int log2_fraction_table_32[32];

//
extern const q32_t atan_table_q32_32_8[32][8];

// computes quadrant/index of q32_t angle
void angle_quadrant_index_q32(q32_t _theta, unsigned int *_quadrant, unsigned int *_index);

//
// Bit manipulation functions
//

// leading zeros (8-bit)
extern const unsigned char lz8[256];

// count leading zeros
#define clz8(_byte) (lz8[_byte])
unsigned int clz(unsigned int _x);

// index of most significant bit
unsigned int msb_index(unsigned int _x);

#endif // __LIQUID_FPM_INTERNAL_H__
