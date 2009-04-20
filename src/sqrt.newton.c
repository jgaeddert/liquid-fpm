//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG

#define LIQUIDFPM_SQRT_NEWTON_NUM_ITERATIONS    10

q32_t q32_sqrt( q32_t _x )
{
    if (_x <= 0) {
        perror("error: q32_sqrt(), x must be > 0");
        return 0;
    }

    q32_t x0 = _x;
    q32_t x1;

    unsigned int i;
    for (i=0; i<LIQUIDFPM_SQRT_NEWTON_NUM_ITERATIONS; i++) {
#ifdef DEBUG
        printf("%4u : %18.14f\n", i, q32_fixed_to_float(x0));
#endif
        x1 = (x0 + q32_div(_x,x0)) / 2;

        if (abs(x1-x0) < 1)
            break;

        x0 = x1;
    }

    //return x1 << (q32_fracbits/2);
    return x1;
}

