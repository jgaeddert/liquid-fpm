//
// fixed|float type operations
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_NEWTON 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

float Q(f_fixed_to_float)(Q(f_t) _x)
{
    return powf(2.0f, _x.base) * Q(_fixed_to_float)(_x.frac);
}

Q(f_t) Q(f_float_to_fixed)(float _x)
{
    Q(f_t) xqf;
    xqf.base = (int)(floorf(log2f(_x)));
    xqf.frac = Q(_float_to_fixed)(_x / (powf(2.0f,xqf.base)));

    return xqf;
}

Q(f_t) Q(f_add)(Q(f_t) _x, Q(f_t) _y)
{
    Q(f_t) sum;
    Q(f_t) max;
    Q(f_t) min;

    // find maximum of _x, _y (ignoring fractional
    // component)
    if (_x.base > _y.base) {
        max.base = _x.base;
        max.frac = _x.frac;

        min.base = _y.base;
        min.frac = _y.frac;
    } else {
        max.base = _y.base;
        max.frac = _y.frac;

        min.base = _x.base;
        min.frac = _x.frac;
    }
    sum.base = max.base;

    // align exponents of arguments
    while (min.base < max.base) {
        min.base++;
        min.frac >>= 1;
    }

    // add fractional components
    sum.frac = min.frac + min.base;

    // constrain
    Q(f_constrain)(sum);

    return sum;
}

Q(f_t) Q(f_sub)(Q(f_t) _x, Q(f_t) _y)
{
    _y.frac = -_y.frac;
    return Q(f_add)(_x,_y);
}

Q(f_t) Q(f_mul)(Q(f_t) _x, Q(f_t) _y)
{
    Q(f_t) prod;
    prod.base = _x.base + _y.base;
    prod.frac = Q(_mul)(_x.frac,_y.frac);

    Q(f_constrain)(prod);

    return prod;
}

Q(f_t) Q(f_div)(Q(f_t) _x, Q(f_t) _y)
{
    unsigned int _n=16;
    Q(f_t) quot;
    quot.base = _x.base - _y.base;
    quot.frac = Q(_div_inv_newton)(_x.frac,_y.frac,_n);

    Q(f_constrain)(quot);

    return quot;
}

// constrain fractional portion: 1 <= fx < 2
Q(f_t) Q(f_constrain)(Q(f_t) _x)
{
    if (_x.frac == 0) {
        printf("error: qfloat_constrain() fractional portion is zero\n");
        exit(0);
    }

    while (Q(_abs)(_x.frac) >= (Q(_one)<<1)) {
        _x.frac >>= 1;
        _x.base++;
    }

    while (Q(_abs)(_x.frac) < Q(_one)) {
        _x.frac <<= 1;
        _x.base--;
    }
}

