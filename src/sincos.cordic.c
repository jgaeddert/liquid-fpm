// 
// sin/cos CORDIC
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SINCOS_CORDIC 0

// polynomial coefficients
//extern const q32_t q32_sine_cordic_tab[q32_bits];

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
    printf("  quadrant : %u\n", q32_quadrant_cordic(_theta));
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


