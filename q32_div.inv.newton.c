//
// q32_div.inv.newton.c
//
// Portable q32_t fixed-point division implementation
//

#include "liquidfpm.internal.h"

#define Q(name)             LIQUIDFPM_CONCAT(q32,name)

// 
Q(_t) Q(_div_inv_newton)(Q(_t) _x,
                         Q(_t) _y,
                         unsigned int _n)
{
    // compute inv(y) = 1/y
    Q(_t) y_inv = Q(_inv_newton)(_y,_n);

    // return x*inv(y)
    return Q(_mul)(_x,y_inv);
}
