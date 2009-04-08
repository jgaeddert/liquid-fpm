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
#define LIQUIDFPM_DEFINE_API(X,T,INTBITS,FRACBITS)                \
typedef T X(_t);      \
X(_t) X(_add)(X(_t) _a, X(_t) _b);

LIQUIDFPM_DEFINE_API(LIQUIDFPM_MANGLE_Q32, int32_t, 4, 28)

/* API definition macro (complex types)
 *
 * X        :   name-mangling macro (complex)
 * Q        :   name-mangling macro (real)
 */
#define LIQUIDFPM_DEFINE_COMPLEX_API(X,Q)               \
typedef struct {Q(_t) real; Q(_t) imag;} X(_t);         \
inline Q(_t) X(_real)(X(_t) _a) {return _a.real;};      \
inline Q(_t) X(_imag)(X(_t) _a) {return _a.imag;};      \
X(_t) X(_add)(X(_t) _a, X(_t) _b);

LIQUIDFPM_DEFINE_COMPLEX_API(LIQUIDFPM_MANGLE_CQ32, LIQUIDFPM_MANGLE_Q32)

#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */

#endif // __LIQUIDFPM_H__
