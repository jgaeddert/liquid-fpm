// 
// sin/cos CORDIC
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SINCOS_CORDIC 1

// cordic coefficients
const q32_t q32_sine_cordic_tab[32] = {
    0x0c90fdb0,    0x076b19c0,    0x03eb6ec0,    0x01fd5baa,
    0x00ffaade,    0x007ff557,    0x003ffeab,    0x001fffd5,
    0x000ffffb,    0x0007ffff,    0x00040000,    0x00020000,
    0x00010000,    0x00008000,    0x00004000,    0x00002000,
    0x00001000,    0x00000800,    0x00000400,    0x00000200,
    0x00000100,    0x00000080,    0x00000040,    0x00000020,
    0x00000010,    0x00000008,    0x00000004,    0x00000002,
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
    if (_theta >= q32_pi)
        _theta -= q32_pi;
    else if (_theta <= -q32_pi)
        _theta += q32_pi;

    unsigned int q = q32_quadrant_cordic(_theta);
#if DEBUG_SINCOS_CORDIC
    printf("  quadrant : %u\n", q);
#endif
    // -pi <= _theta <= pi
    if (q==0) {
    } else if (q==1) {
        _theta -= q32_pi_by_2;
    } else if (q==2) {
        _theta += q32_pi_by_2;
    } else {
    }

    q32_t x = q32_cordic_k_inv;
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
        tz = z - ((q32_sine_cordic_tab[i]^d)-d);
        x = tx;
        y = ty;
        z = tz;
#if DEBUG_SINCOS_CORDIC
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(q32_sine_cordic_tab[i])*(z>=0?1.0:-1.0));
#endif
    }

    // set values according to quadrant
    if (q==0 ) {
        *_cos =  x;
        *_sin =  y;
    } else if (q==1) {
        *_cos = -y;
        *_sin =  x;
    } else if (q==2) {
        *_cos = -x;
        *_sin = -y;
    } else {
        *_cos =  x;
        *_sin =  y;
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


