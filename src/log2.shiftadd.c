// 
// log2 shift/add
//
// Algorithm from fxtbook, ch. 31
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_LOG2_SHIFTADD     0

// natural logarithm
//    ln(x) = log2(x) / ln(2)
//          = log2(x) * log2(e)
q32_t q32_log_shiftadd(q32_t _x, unsigned int _n)
{
    return q32_mul(q32_log2_shiftadd(_x,_n), q32_log2_e);
}

// base-10 logarithm
//    log10(x) = log2(x) / log10(2)
//             = log2(x) * log2(10)
q32_t q32_log10_shiftadd(q32_t _x, unsigned int _n)
{
    return q32_mul(q32_log2_shiftadd(_x,_n), q32_log2_10);
}

// Computes y = log2(x) by pre-shifting the input _x such
// that _x is in [1,2), and then performing the iterative
// shift|add operation on the result.
q32_t q32_log2_shiftadd(q32_t _x,
                        unsigned int _n)
{
    // base index
    int b = msb_index(_x) - 1;

    // compute shift amount
    int s = (int)q32_fracbits - b;

    // pre-shift input (left : s>0, right : s<0)
    q32_t x_hat = s>0 ? _x<<s : _x>>(-s);

#if DEBUG_LOG2_SHIFTADD
    printf("x : %12.8f >> %12.8f\n", q32_fixed_to_float(_x), 
                                     q32_fixed_to_float(x_hat));
    printf("s : %12.8f\n", q32_fixed_to_float(-s<<q32_fracbits));
#endif

    // compute the fractional portion using the iterative
    // shift|add algorithm.
    q32_t yfrac = q32_log2_shiftadd_base(x_hat,_n);

    // compute the integer portion: simply the integer
    // representation of the base index of the original
    // input value _x
    q32_t yint = (-s) << (q32_fracbits);

    return yint + yfrac;
}

// computes y=log2(x) where x >= 1
//
// For values of x < 1, it is necessary to pre-shift x by its
// most-significant bit.  The algorithm will NOT converge for
// x < 1, nor will it validate that the input is in this range.
q32_t q32_log2_shiftadd_base(q32_t _x,
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
#if DEBUG_LOG2_SHIFTADD
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(tn),
            q32_fixed_to_float(en),
            q32_fixed_to_float(q32_log2_shiftadd_Ak_tab[0]));
#endif

    for (i=1; i<n; i++) {
        vn >>= 1;
        while (1) {
            //un = en + q32_mul(en,vn);
            un = en;
            un += en>>i;
            dn = (un <= x);
#if DEBUG_LOG2_SHIFTADD
            printf("%4u %12.8f %12.8f %12.8f %12.4e\n",
                    i,
                    q32_fixed_to_float(un),
                    q32_fixed_to_float(tn),
                    q32_fixed_to_float(en),
                    q32_fixed_to_float(q32_log2_shiftadd_Ak_tab[i]));
#endif
            if (dn == 0) break;
            tn += q32_log2_shiftadd_Ak_tab[i];
            en = un;
        }
    }

    return tn;
}

