// 
// atan | cordic
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_ATAN_CORDIC 1

void q32_atan2_cordic_base(q32_t _y,
                           q32_t _x,
                           q32_t * _r,
                           q32_t * _theta,
                           unsigned int _n);

void q32_atan2_cordic(q32_t _y,
                      q32_t _x,
                      q32_t * _r,
                      q32_t * _theta,
                      unsigned int _n)
{
    // negate calculated angle if _y < 0
    int negate = (_y < 0);

    // subtract calculated angle from pi if _x < 0
    int sub_from_pi = (_x < 0);

    // take absolute value as we are only interested in absolute ratio
    // at this point.
    _y = abs(_y);
    _x = abs(_x);

    q32_atan2_cordic_base(_y,_x,_r,_theta,_n);
    
    if (sub_from_pi)
        *_theta = q32_pi - *_theta;
    if (negate)
        *_theta = -(*_theta);
}

void q32_atan2_cordic_base(q32_t _y,
                           q32_t _x,
                           q32_t * _r,
                           q32_t * _theta,
                           unsigned int _n)
{

    q32_t x = _x;
    q32_t y = _y;
    q32_t z = 0;
    q32_t d,tx,ty,tz;
    unsigned int i;
    unsigned int n=_n;

#if DEBUG_ATAN_CORDIC
    printf("   n            x            y            z        -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            0.0);
#endif
    for (i=0; i<n; i++) {
        d = ( y>=0 ) ? -1 : 0;

        tx = x - ((y>>i)^d)-d;
        ty = y + ((x>>i)^d)-d;
        tz = z - ((q32_cordic_Ak_tab[i]^d)-d);
        x = tx;
        y = ty;
        z = tz;
#if DEBUG_ATAN_CORDIC
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(q32_cordic_Ak_tab[i])*(y>=0?-1.0:1.0));
#endif
    }
    //q32_t x = q32_cordic_k_inv; // TODO : initialize with cordic_Kinv_tab[_n-1];
    *_r = q32_mul(x,q32_cordic_k_inv);
    *_theta = z;
}

