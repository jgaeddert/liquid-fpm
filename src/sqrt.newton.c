//
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG

q32_t q32_sqrt( q32_t _x )
{
    if (_x <= 0) {
        perror("error: q32_sqrt(), x must be > 0");
        return 0;
    }

    int x0 = _x;
    int x1;

    unsigned int i;
    for (i=0; i<20; i++) {
#ifdef DEBUG
        printf("%4u : %d\n", i, x0);
#endif
        x1 = (x0 + _x/x0)/2;
        x0 = x1;
    }

    return x1 << (q32_fracbits/2);
}

