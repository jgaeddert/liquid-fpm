// 
//
//

#ifndef __LIQUID_FPM_INTERNAL_H__
#define __LIQUID_FPM_INTERNAL_H__

#include "liquidfpm.h"
#include "config.h"

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
Q(_t) Q(_sin_cordic)(Q(_t) _theta, unsigned int _n);                \
Q(_t) Q(_cos_cordic)(Q(_t) _theta, unsigned int _n);                \
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
extern const Q(_t) Q(_log2_fraction_table)[256];                    \
                                                                    \
/* */                                                               \
extern const Q(_t) Q(_log2_shiftadd_Ak_tab)[32];                    \
extern const unsigned int Q(_log2_shiftadd_nmax);                   \
extern const Q(_t) Q(_ln2);     /* log(2)   */                      \
extern const Q(_t) Q(_log10_2); /* log10(2) */                      \
extern const Q(_t) Q(_log2_e);  /* log2(e) */                       \
extern const Q(_t) Q(_log2_10); /* log2(10) */                      \
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
Q(_t) Q(_pow_shiftadd)(Q(_t) _b, Q(_t) _x, unsigned int _n);        \
                                                                    \
/* hyperbolic trig */                                               \
extern const Q(_t) Q(_sinhcosh_cordic_Ak_tab)[32];                  \
extern const Q(_t) Q(_sinhcosh_cordic_kp_inv);                      \
void Q(_sinhcosh_cordic)(Q(_t) _x, Q(_t) *_sinh, Q(_t) *_cosh, unsigned int _n);

LIQUIDFPM_DEFINE_INTERNAL_API(LIQUIDFPM_MANGLE_Q32, int32_t, 4, 28)

#endif // __LIQUID_FPM_INTERNAL_H__
