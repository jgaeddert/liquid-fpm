// 
// log2 shift/add
//
// Algorithm from fxtbook, ch. 31
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_LOG2_SHIFTADD     0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// natural logarithm
//    ln(x) = log2(x) / log2(e)
//          = log2(x) * log(2)
Q(_t) Q(_log_shiftadd)(Q(_t) _x, unsigned int _n)
{
    return Q(_mul)(Q(_log2_shiftadd)(_x,_n), Q(_ln2));
}

// base-10 logarithm
//    log10(x) = log2(x) / log2(10)
//             = log2(x) * log10(2)
Q(_t) Q(_log10_shiftadd)(Q(_t) _x, unsigned int _n)
{
    return Q(_mul)(Q(_log2_shiftadd)(_x,_n), Q(_log10_2));
}

// Computes y = log2(x) by pre-shifting the input _x such
// that _x is in [1,2), and then performing the iterative
// shift|add operation on the result.
Q(_t) Q(_log2_shiftadd)(Q(_t) _x,
                        unsigned int _n)
{
    // base index
    int b = msb_index(_x) - 1;

    // compute shift amount
    int s = (int)Q(_fracbits) - b;

    // pre-shift input (left : s>0, right : s<0)
    Q(_t) x_hat = s>0 ? _x<<s : _x>>(-s);

#if DEBUG_LOG2_SHIFTADD
    printf("x : %12.8f >> %12.8f\n", Q(_fixed_to_float)(_x), 
                                     Q(_fixed_to_float)(x_hat));
    printf("s : %12.8f\n", Q(_fixed_to_float)(-s<<Q(_fracbits)));
#endif

    // compute the fractional portion using the iterative
    // shift|add algorithm.
    Q(_t) yfrac = Q(_log2_shiftadd_base)(x_hat,_n);

    // compute the integer portion: simply the integer
    // representation of the base index of the original
    // input value _x
    Q(_t) yint = (-s) << (Q(_fracbits));

    return yint + yfrac;
}

// computes y=log2(x) where x >= 1
//
// For values of x < 1, it is necessary to pre-shift x by its
// most-significant bit.  The algorithm will NOT converge for
// x < 1, nor will it validate that the input is in this range.
Q(_t) Q(_log2_shiftadd_base)(Q(_t) _x,
                             unsigned int _n)
{
    Q(_t) tn = 0;
    Q(_t) en = Q(_one);
    Q(_t) un = 0;
    Q(_t) vn = Q(_one);
    Q(_t) x = _x;
    int dn;
    unsigned int n = _n;
    if (n>Q(_log2_shiftadd_nmax))
        n = Q(_log2_shiftadd_nmax);
    unsigned int i;
#if DEBUG_LOG2_SHIFTADD
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n",
            Q(_fixed_to_float)(tn),
            Q(_fixed_to_float)(en),
            Q(_fixed_to_float)(Q(_log2_shiftadd_Ak_tab)[0]));
#endif

    for (i=1; i<n; i++) {
        vn >>= 1;
        while (1) {
            //un = en + Q(_mul)(en,vn);
            un = en;
            un += en>>i;
            dn = (un <= x);
#if DEBUG_LOG2_SHIFTADD
            printf("%4u %12.8f %12.8f %12.8f %12.4e\n",
                    i,
                    Q(_fixed_to_float)(un),
                    Q(_fixed_to_float)(tn),
                    Q(_fixed_to_float)(en),
                    Q(_fixed_to_float)(Q(_log2_shiftadd_Ak_tab)[i]));
#endif
            if (dn == 0) break;
            tn += Q(_log2_shiftadd_Ak_tab)[i];
            en = un;
        }
    }

    return tn;
}

