//
// math : transcendental functions
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_MATH_TRANSCENDENTALS 1

#define Q(name)         LIQUIDFPM_CONCAT(q32,name)

// log(gamma(z))
Q(_t) Q(_lngamma)(Q(_t) _z)
{
    return 0;
}

// gamma(z) = exp(lngamma(z))
Q(_t) Q(_gamma)(Q(_t) _z)
{
    return Q(_log)( Q(_lngamma)(_z) );
}

// BesselI_0(z)
Q(_t) Q(_besseli0)(Q(_t) _z)
{
    return 0;
}

// BesselJ_0(z)
Q(_t) Q(_besselj0)(Q(_t) _z)
{
    return 0;
}

// sinc(z) = sin(pi*z)/(pi*z)
Q(_t) Q(_sinc)(Q(_t) _z)
{
    // z ~ 0 approximation
    // sinc(z) = \prod_{k=1}^{\infty}{ cos(\pi z / 2^k) }
    if (Q(_abs)(_z) < (Q(_one)>>6))
        return 0;

    return 0;
}

