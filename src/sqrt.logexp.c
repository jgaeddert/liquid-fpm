//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_LOGEXP 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

//
// use log2 and exp2 functions to approximate square root
//
Q(_t) Q(_sqrt_logexp)( Q(_t) _x, unsigned int _n )
{
    if (_x <= 0) {
        perror("error: q[type]_sqrt(), x must be > 0");
        return 0;
    }

    // TODO : ensure precision is maintained by splitting log2
    //        output into base and fractional components

    // compute logarithm
    Q(_t) log2x = Q(_log2)(_x);

    // divide by 2 (logical bit shift)
    Q(_t) log2x_by_2 = log2x >> 1;

    // compute exponential
    Q(_t) qsqrt = Q(_exp2)(log2x_by_2);

#if DEBUG_SQRT_LOGEXP
    printf("    x           = %12.10f\n", Q(_fixed_to_float)(_x));
    printf("    log2(x)     = %12.10f\n", Q(_fixed_to_float)(log2x));
    printf("    log2(x)/2   = %12.10f\n", Q(_fixed_to_float)(log2x_by_2));
    printf("    sqrt(x)     = %12.10f\n", Q(_fixed_to_float)(qsqrt));
#endif
    return qsqrt;
}

