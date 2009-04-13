/*
 * Copyright (c) 2008, 2009 Joseph Gaeddert
 * Copyright (c) 2008, 2009 Virginia Polytechnic Institute & State University
 */

#ifndef __LIQUIDFPM_H__
#define __LIQUIDFPM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <inttypes.h>

/* concatenation function */
#define LIQUIDFPM_CONCAT(prefix, name) prefix ## name

/* name-mangling macros */
#define LIQUIDFPM_MANGLE_Q32(name)  LIQUIDFPM_CONCAT(q32, name)
#define LIQUIDFPM_MANGLE_CQ32(name) LIQUIDFPM_CONCAT(cq32,name)

/* API definition macro
 *
 * X        :   name-mangling macro
 * T        :   primitive data type
 * INTBITS  :   number of integer bits
 * FRACBITS :   number of fractional bits
 */
#define LIQUIDFPM_DEFINE_API(X,T,INTBITS,FRACBITS)                  \
typedef T X(_t);                                                    \
const static unsigned int X(_intbits) = INTBITS;                    \
const static unsigned int X(_fracbits) = FRACBITS;                  \
                                                                    \
/* conversion */                                                    \
static inline float X(_fixed_to_float)(X(_t) _x)                    \
    { return (float) (_x) / (float)(1 << FRACBITS); };              \
static inline X(_t) X(_float_to_fixed)(float _x)                    \
    { return (X(_t)) (_x * (float)(1 << FRACBITS) + 0.5f); };       \
                                                                    \
/* arithmetic */                                                    \
static inline X(_t) X(_abs)(X(_t) _x) {return (_x<0) ? -_x : _x;};  \
static inline X(_t) X(_add)(X(_t) _a, X(_t) _b) {return _a + _b;};  \
static inline X(_t) X(_sub)(X(_t) _a, X(_t) _b) {return _a - _b;};  \
X(_t) X(_mul)(X(_t) _a, X(_t) _b);                                  \
X(_t) X(_div)(X(_t) _a, X(_t) _b);                                  \
                                                                    \
/* trig */                                                          \
X(_t) X(_sin)(X(_t) _theta);                                        \
X(_t) X(_cos)(X(_t) _theta);                                        \
void  X(_sincos)(X(_t) _theta, X(_t) * _sin, X(_t) * _cos);         \
X(_t) X(_tan)(X(_t) _theta);                                        \
X(_t) X(_atan2)(X(_t) _x, X(_t) _y);                                \
                                                                    \
/* log, etc. */                                                     \
X(_t) X(_log2)(X(_t) _x);                                           \
X(_t) X(_exp) (X(_t) _x);                                           \
X(_t) X(_sqrt)(X(_t) _x);                                           \
X(_t) X(_pow) (X(_t) _x);                                           \
                                                                    \
/* vector operations */                                             \
X(_t) X(_dotprod)(X(_t) * _x, X(_t) * _v, unsigned int _n);


LIQUIDFPM_DEFINE_API(LIQUIDFPM_MANGLE_Q32, int32_t, 4, 28)

/* API definition macro (complex types)
 *
 * X        :   name-mangling macro (complex)
 * Q        :   name-mangling macro (real)
 */
#define LIQUIDFPM_DEFINE_COMPLEX_API(X,Q)                           \
typedef struct {Q(_t) real; Q(_t) imag;} X(_t);                     \
                                                                    \
/* accessor functions */                                            \
static inline Q(_t) X(_real)(X(_t) _a) {return _a.real;};           \
static inline Q(_t) X(_imag)(X(_t) _a) {return _a.imag;};           \
X(_t) X(_conj)(X(_t) _a);                                           \
                                                                    \
/* arithmetic */                                                    \
X(_t) X(_add)(X(_t) _a, X(_t) _b);                                  \
X(_t) X(_sub)(X(_t) _a, X(_t) _b);                                  \
X(_t) X(_mul)(X(_t) _a, X(_t) _b);                                  \
X(_t) X(_div)(X(_t) _a, X(_t) _b);                                  \
                                                                    \
/* trig */                                                          \
X(_t) X(_cexp)(X(_t) _x);                                           \
Q(_t) X(_carg)(X(_t) _x);/* { return Q(_atan2)(_x.imag, _x.real); }; */   \
Q(_t) X(_abs) (X(_t) _x);

LIQUIDFPM_DEFINE_COMPLEX_API(LIQUIDFPM_MANGLE_CQ32, LIQUIDFPM_MANGLE_Q32)

#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */

#endif // __LIQUIDFPM_H__
