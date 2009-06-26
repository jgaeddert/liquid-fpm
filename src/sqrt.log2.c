//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_LOG2 0

//
// use log2 and exp2 functions to approximate sqare root
//
q32_t q32_sqrt( q32_t _x )
{
    if (_x <= 0) {
        perror("error: q32_sqrt(), x must be > 0");
        return 0;
    }

    // compute logarithm
    q32_t log2x = q32_log2(_x);

    // divide by 2 (multiply by 1/2)
    q32_t log2x_by_2 = q32_mul(log2x,(1<<(q32_fracbits-1)));

    // compute exponential
    q32_t qsqrt = q32_exp2(log2x_by_2);

#if DEBUG_SQRT_LOG2
    printf("    x           = %12.10f\n", q32_fixed_to_float(_x));
    printf("    log2(x)     = %12.10f\n", q32_fixed_to_float(log2x));
    printf("    log2(x)/2   = %12.10f\n", q32_fixed_to_float(log2x_by_2));
    printf("    sqrt(x)     = %12.10f\n", q32_fixed_to_float(qsqrt));
#endif
    return qsqrt;
}

