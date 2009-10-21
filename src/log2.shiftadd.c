// 
// log2 shift/add
//
// Algorithm from fxtbook, ch. 31
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_LOG2_SHIFTADD     1

// Ak = log2( 1 + 2^-k )
const q32_t q32_log2_shiftadd_Ak_tab[32] = {
    0x10000000,    0x095c01a0,    0x05269e10,    0x02b80348,
    0x01663f70,    0x00b5d69c,    0x005b9e5a,    0x002dfca2,
    0x001709c4,    0x000b87c2,    0x0005c499,    0x0002e27b,
    0x00017149,    0x0000b8a7,    0x00005c54,    0x00002e2a,
    0x00001715,    0x00000b8b,    0x000005c5,    0x000002e3,
    0x00000171,    0x000000b9,    0x0000005c,    0x0000002e,
    0x00000017,    0x0000000c,    0x00000006,    0x00000003,
    0x00000001,    0x00000001,    0x00000000,    0x00000000};

// Maximum number of iterations, given the shiftadd_Ak_table
// above.  The shift|add algorithm will hit an infinite loop
// condition for values in the table equal to zero, hence this
// limitation.
const q32_t q32_log2_shiftadd_nmax = 29;

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

    q32_t y = q32_log2_shiftadd_base(x_hat,_n);

    y += -s<<(q32_fracbits);

    return y;
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

