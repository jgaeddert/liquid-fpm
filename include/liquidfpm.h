/*
 * Copyright (c) 2008, 2009 Joseph Gaeddert
 * Copyright (c) 2008, 2009 Virginia Polytechnic Institute & State University
 */

#ifndef __LIQUIDFPM_H__
#define __LIQUIDFPM_H__

#ifdef __cplusplus
extern "C" {

#else

#endif /* __cplusplus */

/* concatenation function */
#define LIQUIDFPM_CONCAT(prefix, name) prefix ## name

/* name-mangling macros */
#define LIQUIDFPM_MANGLE_Q32(name)  LIQUIDFPM_CONCAT(lfpm_q4_28, name)

/* API definition macro
 *
 * X        :   name-mangling macro
 * T        :   primitive data type
 * INTBITS  :   number of integer bits
 * FRACBITS :   number of fractional bits
 */
#define LIQUIDFPM_DEFINE_API(X,T,INTBITS,FRACBITS)                \
typedef T X();      \
X() X(_add)(X() _a, X() _b);

LIQUIDFPM_DEFINE_API(LIQUIDFPM_MANGLE_Q32, int32_t, 3, 28)

#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */

#endif // __LIQUIDFPM_H__
