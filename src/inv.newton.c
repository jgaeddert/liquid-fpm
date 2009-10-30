//
// inverse approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_INV_NEWTON 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// computes x = inv(d) = 1/d using iterative Newtonian method:
//   x[k+1] = x[k] + x[k]*(1 - d*x[k])
Q(_t) Q(_inv_newton)( Q(_t) _x, unsigned int _n )
{
    int negate = (_x < 0);
    _x = Q(_abs)(_x);

    // initial guess: x0 = 2^-floor(log2(|_x|))
    int b = msb_index(_x) - 1;          // base index
    int s = (int)Q(_fracbits) - b - 1;  // shift amount
    Q(_t) x0 = s>0 ? Q(_one)<<s : Q(_one)>>(-s);

    Q(_t) x1=0;
    Q(_t) y0=0;
    Q(_t) d=_x;
    Q(_t) dx0=0;
    Q(_t) x0y0=0;

    unsigned int i;
#if DEBUG_INV_NEWTON
    printf("   x : %12.8f\n", Q(_fixed_to_float)(_x));
    printf("  x0 : %12.8f\n", Q(_fixed_to_float)(x0));
    printf("   n :         d*x0           y0        x0*y0           x1\n");
#endif
    for (i=0; i<_n; i++) {
#if DEBUG_INV_NEWTON
        printf("%4u : %12.8f %12.8f %12.8f %12.8f\n", i,
                                 Q(_fixed_to_float)(dx0),
                                 Q(_fixed_to_float)(y0),
                                 Q(_fixed_to_float)(x0y0),
                                 Q(_fixed_to_float)(x1));
#endif
        dx0  = Q(_mul)(d,x0);
        y0   = Q(_one) - dx0;
        x0y0 = Q(_mul)(x0,y0);
        x1   = x0 + x0y0;

        // break if multiplier is zero: all further
        // iterations will result in the same value
        // for x1
        if (!y0 || !x0y0) break;

        x0 = x1;
    }

    return negate ? -x1 : x1;
}

