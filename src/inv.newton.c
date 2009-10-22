//
// inverse approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_INV_NEWTON 1

// computes x = inv(d) = 1/d using iterative Newtonian method:
//   x[k+1] = x[k] + x[k]*(1 - d*x[k])
q32_t q32_inv( q32_t _x, unsigned int _n )
{
    // initial guess
    // TODO : choose better starting value
    q32_t x0 = q32_one;

    q32_t x1=0;
    q32_t y0=0;
    q32_t d=_x;
    q32_t dx0=0;
    q32_t x0y0=0;

    unsigned int i;
#if DEBUG_INV_NEWTON
    printf("   n :         d*x0           y0        x0*y0           x1\n");
#endif
    for (i=0; i<_n; i++) {
#if DEBUG_INV_NEWTON
        printf("%4u : %12.8f %12.8f %12.8f %12.8f\n", i,
                                 q32_fixed_to_float(dx0),
                                 q32_fixed_to_float(y0),
                                 q32_fixed_to_float(x0y0),
                                 q32_fixed_to_float(x1));
#endif
        dx0  = q32_mul(d,x0);
        y0   = q32_one - dx0;
        x0y0 = q32_mul(x0,y0);
        x1   = x0 + x0y0;

        x0 = x1;
    }

    return x1;
}

