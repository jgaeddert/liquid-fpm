//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_NEWTON 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

Q(_t) Q(_sqrt_newton)( Q(_t) _x, unsigned int _n)
{
    if (_x < 0) {
        printf("error: qtype_sqrt_newton(), x must be >= 0");
        return 0;
    }

    if (_x == 0)
        return 0;

    // initial guess: x0 = _x
    //Q(_t) x0 = _x;

    // initial guess: set x0 to 2^floor(msb_index/2)
    // NOTE: we need to compensate for the decimal position of q32
    Q(_t) x0 = 1 << (((msb_index(_x)-Q(_fracbits))/2)+Q(_fracbits));

    Q(_t) x1=0;

    unsigned int i;
    for (i=0; i<_n; i++) {
#if DEBUG_SQRT_NEWTON
        printf("%4u : %18.14f\n", i, Q(_fixed_to_float)(x0));
#endif
        x1 = (x0 + Q(_div)(_x,x0)) / 2;

        // break if change in this iteration is negligible
        if (abs(x1-x0) < 1)
            break;

        x0 = x1;
    }

    //return x1 << (Q(_fracbits)/2);
    return Q(_abs)(x1);
}

