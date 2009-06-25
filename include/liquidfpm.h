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
#include <complex.h>

/* concatenation function */
#define LIQUIDFPM_CONCAT(prefix, name) prefix ## name

/* name-mangling macros */
#define LIQUIDFPM_MANGLE_Q32(name)  LIQUIDFPM_CONCAT(q32, name)
#define LIQUIDFPM_MANGLE_CQ32(name) LIQUIDFPM_CONCAT(cq32,name)

/* API definition macro
 *
 * Q        :   name-mangling macro
 * T        :   primitive data type
 * INTBITS  :   number of integer bits
 * FRACBITS :   number of fractional bits
 */
#define LIQUIDFPM_DEFINE_API(Q,T,INTBITS,FRACBITS)                  \
typedef T Q(_t);                                                    \
const static unsigned int Q(_intbits) = INTBITS;                    \
const static unsigned int Q(_fracbits) = FRACBITS;                  \
static inline int Q(_intpart) (Q(_t) _x)                            \
    { return (_x >> FRACBITS); } ;                                  \
static inline int Q(_fracpart) (Q(_t) _x)                           \
    { return _x & ((1<<FRACBITS)-1); };                             \
                                                                    \
/* constants */                                                     \
const static Q(_t) Q(_min) = (1);                                   \
const static Q(_t) Q(_max) = (1<<(FRACBITS));                       \
const static Q(_t) Q(_pi) = (1<<(INTBITS+FRACBITS-1))-1;            \
                                                                    \
/* conversion */                                                    \
static inline float Q(_fixed_to_float)(Q(_t) _x)                    \
    { return (float) (_x) / (float)(1 << FRACBITS); };              \
static inline Q(_t) Q(_float_to_fixed)(float _x)                    \
    { return (Q(_t)) (_x * (float)(1 << FRACBITS) + 0.5f); };       \
                                                                    \
/* arithmetic */                                                    \
static inline Q(_t) Q(_abs)(Q(_t) _x) {return (_x<0) ? -_x : _x;};  \
static inline Q(_t) Q(_add)(Q(_t) _a, Q(_t) _b) {return _a + _b;};  \
static inline Q(_t) Q(_sub)(Q(_t) _a, Q(_t) _b) {return _a - _b;};  \
Q(_t) Q(_mul)(Q(_t) _a, Q(_t) _b);                                  \
Q(_t) Q(_div)(Q(_t) _a, Q(_t) _b);                                  \
                                                                    \
/* trig */                                                          \
Q(_t) Q(_sin)(Q(_t) _theta);                                        \
Q(_t) Q(_cos)(Q(_t) _theta);                                        \
void  Q(_sincos)(Q(_t) _theta, Q(_t) * _sin, Q(_t) * _cos);         \
Q(_t) Q(_tan)(Q(_t) _theta);                                        \
Q(_t) Q(_atan2)(Q(_t) _x, Q(_t) _y);                                \
                                                                    \
/* log, etc. */                                                     \
typedef struct {int base; Q(_t) frac;} Q(_log2_t);                  \
Q(_t) Q(_log2)(Q(_t) _x);                                           \
Q(_t) Q(_exp2)(Q(_t) _x);                                           \
Q(_t) Q(_log) (Q(_t) _x);                                           \
Q(_t) Q(_exp) (Q(_t) _x);                                           \
Q(_t) Q(_sqrt)(Q(_t) _x);                                           \
Q(_t) Q(_pow) (Q(_t) _x);                                           \
                                                                    \
/* vector operations */                                             \
Q(_t) Q(_dotprod)(Q(_t) * _x, Q(_t) * _v, unsigned int _n);


LIQUIDFPM_DEFINE_API(LIQUIDFPM_MANGLE_Q32, int32_t, 4, 28)

/* API definition macro (complex types)
 *
 * CQ       :   name-mangling macro (complex)
 * Q        :   name-mangling macro (real)
 */
#define LIQUIDFPM_DEFINE_COMPLEX_API(CQ,Q)                          \
typedef struct {Q(_t) real; Q(_t) imag;} CQ(_t);                    \
                                                                    \
/* accessor functions */                                            \
static inline Q(_t) CQ(_real)(CQ(_t) _a) {return _a.real;};         \
static inline Q(_t) CQ(_imag)(CQ(_t) _a) {return _a.imag;};         \
CQ(_t) CQ(_conj)(CQ(_t) _a);                                        \
                                                                    \
/* conversion */                                                    \
float complex CQ(_fixed_to_float)(CQ(_t) _x);                       \
CQ(_t) CQ(_float_to_fixed)(float complex _x);                       \
                                                                    \
/* arithmetic */                                                    \
CQ(_t) CQ(_add)(CQ(_t) _a, CQ(_t) _b);                              \
CQ(_t) CQ(_sub)(CQ(_t) _a, CQ(_t) _b);                              \
CQ(_t) CQ(_mul)(CQ(_t) _a, CQ(_t) _b);                              \
CQ(_t) CQ(_div)(CQ(_t) _a, CQ(_t) _b);                              \
                                                                    \
/* trig */                                                          \
CQ(_t) CQ(_cexp)(CQ(_t) _x);                                        \
CQ(_t) CQ(_clog)(CQ(_t) _x);                                        \
static inline Q(_t) CQ(_carg)(CQ(_t) _x)                            \
    { return Q(_atan2)(_x.imag, _x.real); };                        \
Q(_t) CQ(_abs2) (CQ(_t) _x);                                        \
static inline Q(_t) CQ(_abs) (CQ(_t) _x)                            \
    {return Q(_sqrt)(CQ(_abs2)(_x));};

LIQUIDFPM_DEFINE_COMPLEX_API(LIQUIDFPM_MANGLE_CQ32, LIQUIDFPM_MANGLE_Q32)

#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */

#endif // __LIQUIDFPM_H__
