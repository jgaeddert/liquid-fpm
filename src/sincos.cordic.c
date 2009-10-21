// 
// sin/cos CORDIC
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SINCOS_CORDIC 0

// cordic coefficients
const q32_t q32_cordic_Ak_tab[32] = {
    0x10000000,    0x09720290,    0x04fd9c28,    0x028888e8,
    0x014586a0,    0x00a2ebf0,    0x00517b0f,    0x0028be2b,
    0x00145f2a,    0x000a2f97,    0x000517cc,    0x00028be6,
    0x000145f3,    0x0000a2fa,    0x0000517d,    0x000028be,
    0x0000145f,    0x00000a30,    0x00000518,    0x0000028c,
    0x00000146,    0x000000a3,    0x00000051,    0x00000029,
    0x00000014,    0x0000000a,    0x00000005,    0x00000003,
    0x00000001,    0x00000001,    0x00000000,    0x00000000};

const q32_t q32_cordic_k_inv = 0x09b74ee0;

unsigned int q32_quadrant_cordic(q32_t _theta)
{
    // Ensure theta >= 0
    // Because the Q4.28 representation of angles is [-2*pi,2*pi] we can guarantee
    // adding 2*pi will make theta non-negative
    if ( _theta < 0 )
        _theta += q32_2pi;

    // _theta now has the form : 0QQB BBBB BBBx xxxx xxxx xxxx xxxx xxxx
    // Q : 2-bit quadrant
    // B : 8-bit phase resolution
    // x : remaining 21 bits phase resolution, ignored

    // Extract quadrant
    //   1. Shift by 29, len(x) + len(B)
    //   2. Mask with 0x0003 to ensure 2-bit value
    return (_theta >> (q32_bits-3)) & 0x00000003;
}

void q32_sincos_cordic(q32_t _theta,
                       q32_t * _sin,
                       q32_t * _cos,
                       unsigned int _n)
{
    // constrain input angle to be in [-pi, pi]
    // TODO : determine more efficient way of doing this
    if (_theta >= q32_pi)
        _theta -= q32_2pi;
    else if (_theta <= -q32_pi)
        _theta += q32_2pi;

    // invert phase: constrain to [-pi/2,pi/2]
    // This is necessary because the cordic will only converge
    // if -r < _theta < r, where
    //      r = sum(k=0, infty) ( arctan(2^-k) )
    //      r ~ 1.743286620472340
    // Because r > pi/2 (1.57079632), constraining the phase
    // to be in [-pi/2,pi/2] will ensure convergence.
    int invert=0;
    if (_theta > q32_pi_by_2) {
        _theta -= q32_pi;
        invert = 1;
    } else if( _theta < -q32_pi_by_2) {
        _theta += q32_pi;
        invert = 1;
    } else {
        invert = 0;
    }

#if DEBUG_SINCOS_CORDIC
    printf("  theta : %12.8f\n", q32_angle_fixed_to_float(_theta));
#endif

    q32_t x = q32_cordic_k_inv; // TODO : initialize with cordic_Kinv_tab[_n-1];
    q32_t y = 0;
    q32_t z = _theta;
    q32_t d,tx,ty,tz;
    unsigned int i;
    unsigned int n=_n;

#if DEBUG_SINCOS_CORDIC
    printf("   n           x            y            z         -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            0.0);
#endif
    for (i=0; i<n; i++) {
        d = ( z>=0 ) ? 0 : -1;
        // d = z >> 31;

        tx = x - ((y>>i)^d)-d;
        ty = y + ((x>>i)^d)-d;
        tz = z - ((q32_cordic_Ak_tab[i]^d)-d);
        x = tx;
        y = ty;
        z = tz;
#if DEBUG_SINCOS_CORDIC
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(q32_cordic_Ak_tab[i])*(z>=0?1.0:-1.0));
#endif
    }

    // negate values if phase has been inverted
    if (!invert) {
        // returning values without negation
        *_cos =  x;
        *_sin =  y;
    } else {
        // negating values due to phase inversion
        *_cos = -x;
        *_sin = -y;
    }
}

#if 0
q32_t q32_sin( q32_t _theta )
{
    return 0;
}

q32_t q32_cos( q32_t _theta )
{
    return 0;
}

void q32_sincos( q32_t _theta, q32_t * _sin, q32_t * _cos )
{
}
#endif


