// 
// exp2 shift/add
//
// Algorithm from fxtbook, ch. 31
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_EXP2_SHIFTADD     0

// natural logarithm
//    e^(x) = 2^(x * log2(e))
q32_t q32_exp_shiftadd(q32_t _x, unsigned int _n)
{
    return q32_log2_shiftadd( q32_mul(_x,q32_log2_e), _n );
}

// base-10 logarithm
//    10^(x) = 2^(x * log2(10))
q32_t q32_exp10_shiftadd(q32_t _x, unsigned int _n)
{
    return q32_log2_shiftadd( q32_mul(_x,q32_log2_10), _n );
}

// Computes y = exp2(x)
q32_t q32_exp2_shiftadd(q32_t _x,
                        unsigned int _n)
{
    if (_x >= 0)
        return q32_exp2_shiftadd_base(_x,_n);

    // base, fraction
    int b   = q32_intpart(_x);
    q32_t f = q32_fracpart(_x);

#if DEBUG_EXP2_SHIFTADD
    printf("x : %12.8f  > %12.8f\n", q32_fixed_to_float(_x), 
                                     q32_fixed_to_float(f));
    printf("b : %d\n", b);
    printf("f : %f\n", q32_fixed_to_float(f));
#endif

    // compute the fractional portion using the iterative
    // shift|add algorithm.
    q32_t y = q32_exp2_shiftadd_base(f,_n);

    // post-shift the output by the integer amount (effectively
    // multiply by 2^-b)
    y >>= -b;

    return y;
}

// computes y=exp2(x) where x >= 0
//
// For values of x < 0, it is necessary to first subtract its
// integer component (i.e. compute x = b+f s.t. b is a negative
// integer and f>0).  The algorithm will NOT converge for
// x < 0, nor will it validate that the input is in this range.
q32_t q32_exp2_shiftadd_base(q32_t _x,
                             unsigned int _n)
{
    q32_t tn = 0;
    q32_t en = q32_one;
    q32_t un = 0;
    q32_t vn = q32_one;
    q32_t x = _x;
    int dn;
    unsigned int n = _n;
    if (n>q32_log2_shiftadd_nmax)
        n = q32_log2_shiftadd_nmax;
    unsigned int i;
#if DEBUG_EXP2_SHIFTADD
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(tn),
            q32_fixed_to_float(en),
            q32_fixed_to_float(q32_log2_shiftadd_Ak_tab[0]));
#endif

    for (i=1; i<n; i++) {
        vn >>= 1;
        while (1) {
            un = tn + q32_log2_shiftadd_Ak_tab[i];
            dn = (un <= x);
#if DEBUG_EXP2_SHIFTADD
            printf("%4u %12.8f %12.8f %12.8f %12.4e\n",
                    i,
                    q32_fixed_to_float(un),
                    q32_fixed_to_float(tn),
                    q32_fixed_to_float(en),
                    q32_fixed_to_float(q32_log2_shiftadd_Ak_tab[i]));
#endif
            if (dn == 0) break;
            tn = un;
            en += en>>i;
        }
    }

    return en;
}

