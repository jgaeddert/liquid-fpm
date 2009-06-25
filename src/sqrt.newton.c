//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_NEWTON 0

#define LIQUIDFPM_SQRT_NEWTON_NUM_ITERATIONS    3

q32_t q32_sqrt( q32_t _x )
{
    if (_x < 0) {
        perror("error: q32_sqrt(), x must be > 0");
        return 0;
    }

    // initial guess: x0 = _x
    //q32_t x0 = _x;

    // initial guess: set x0 to 2^floor(msb_index/2)
    // NOTE: we need to compensate for the decimal position of q32
    q32_t x0 = 1 << (((msb_index(_x)-q32_fracbits)/2)+q32_fracbits);

    q32_t x1;

    unsigned int i;
    for (i=0; i<LIQUIDFPM_SQRT_NEWTON_NUM_ITERATIONS; i++) {
#if DEBUG_SQRT_NEWTON
        printf("%4u : %18.14f\n", i, q32_fixed_to_float(x0));
#endif
        x1 = (x0 + q32_div(_x,x0)) / 2;

        // break if change in this iteration is negligible
        if (abs(x1-x0) < 1)
            break;

        x0 = x1;
    }

    //return x1 << (q32_fracbits/2);
    return q32_abs(x1);
}

