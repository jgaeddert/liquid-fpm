// 
//
//

#ifndef __LIQUID_FPM_INTERNAL_H__
#define __LIQUID_FPM_INTERNAL_H__

#include "liquidfpm.h"
#include "config.h"

//
// Trigonometric helper functions, arrays, constants
// TODO: auto-compute these values
//
#define FPM_Q32_2_PI       0x7FFFFFFF  ///< \f$2\pi\f$
#define FPM_Q32_PI         0x3FFFFFFF  ///< \f$\pi\f$
#define FPM_Q32_PI_BY_2    0x1FFFFFFF  ///< \f$\pi/2\f$
#define FPM_Q32_PI_BY_4    0x0FFFFFFF  ///< \f$\pi/4\f$

#define FPM_Q32_1_BY_SQRT_2 0x0B504F33 ///< \f$ 1/sqrt(2) \f$

//#define q32_angle_scalar (M_PI/4.0f)
//#define q32_angle_fixed_to_float(x) (q32_fixed_to_float(x) * q32_angle_scalar)
//#define q32_angle_float_to_fixed(x) (q32_float_to_fixed(x /  q32_angle_scalar)-1)

//extern const q32_t sin_table_q32_256[256];

// fractional portion, \f$ f \f$ of log2, \f$ log2(x) = b + f/256 \f$
//extern const unsigned int log2_fraction_table_256[256];
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

/* Internal API definition macro
 *
 * Q        :   name-mangling macro
 * T        :   primitive data type
 * INTBITS  :   number of integer bits
 * FRACBITS :   number of fractional bits
 */
#define LIQUIDFPM_DEFINE_INTERNAL_API(Q,T,INTBITS,FRACBITS)         \
                                                                    \
/* auto-generated look-up tables */                                 \
extern const Q(_t) Q(_sin_table)[256];                              \
                                                                    \
/* arithmetic */                                                    \
Q(_t) Q(_inv_newton)(Q(_t), unsigned int _n);                       \
Q(_t) Q(_inv_logexp)(Q(_t), unsigned int _n);                       \
Q(_t) Q(_sqrt_newton)(Q(_t), unsigned int _n);                      \
Q(_t) Q(_sqrt_logexp)(Q(_t), unsigned int _n);                      \
                                                                    \
/* trig */                                                          \
extern const Q(_t) Q(_cordic_Ak_tab)[32];                           \
extern const Q(_t) Q(_cordic_k_inv);                                \
void Q(_sincos_cordic)(Q(_t) _theta,                                \
                       Q(_t) * _sin,                                \
                       Q(_t) * _cos,                                \
                       unsigned int _n);                            \
void Q(_sincos_cordic_base)(Q(_t) _theta,                           \
                            Q(_t) * _sin,                           \
                            Q(_t) * _cos,                           \
                            unsigned int _n);                       \
void Q(_atan2_cordic)(Q(_t) _y,                                     \
                      Q(_t) _x,                                     \
                      Q(_t) * _r,                                   \
                      Q(_t) * _theta,                               \
                      unsigned int _n);                             \
void Q(_atan2_cordic_base)(Q(_t) _y,                                \
                           Q(_t) _x,                                \
                           Q(_t) * _r,                              \
                           Q(_t) * _theta,                          \
                           unsigned int _n);                        \
                                                                    \
/* log (old) */                                                     \
Q(_t) Q(_log2_convert_forward) (Q(_log2_t) _logtype);               \
Q(_log2_t) Q(_log2_convert_reverse) (Q(_t) _x);                     \
extern const Q(_t) Q(_log2_fraction_table)[256];                    \
                                                                    \
/* */                                                               \
extern const Q(_t) Q(_log2_shiftadd_Ak_tab)[32];                    \
extern const unsigned int Q(_log2_shiftadd_nmax);                   \
extern const Q(_t) Q(_log2_e);                                      \
extern const Q(_t) Q(_log2_10);                                     \
                                                                    \
/* log2 shift|add */                                                \
Q(_t) Q(_log2_shiftadd)(Q(_t) _x, unsigned int _n);                 \
Q(_t) Q(_log2_shiftadd_base)(Q(_t) _x, unsigned int _n);            \
Q(_t) Q(_log_shiftadd)(Q(_t) _x, unsigned int _n);                  \
Q(_t) Q(_log10_shiftadd)(Q(_t) _x, unsigned int _n);                \
                                                                    \
/* exp2 shift|add */                                                \
Q(_t) Q(_exp2_shiftadd)(Q(_t) _x, unsigned int _n);                 \
Q(_t) Q(_exp2_shiftadd_base)(Q(_t) _x, unsigned int _n);            \
Q(_t) Q(_exp_shiftadd)(Q(_t) _x, unsigned int _n);                  \
Q(_t) Q(_exp10_shiftadd)(Q(_t) _x, unsigned int _n);                \
                                                                    \
Q(_t) Q(_pow_shiftadd)(Q(_t) _b, Q(_t) _x, unsigned int _n);

LIQUIDFPM_DEFINE_INTERNAL_API(LIQUIDFPM_MANGLE_Q32, int32_t, 4, 28)

#endif // __LIQUID_FPM_INTERNAL_H__
