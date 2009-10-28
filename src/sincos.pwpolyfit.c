// 
// sin/cos piecewise poly
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SINCOS_PWPOLY 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// polynomial coefficients
Q(_t) Q(_sine_table_pwpolyfit)[16][3] = {
    {0xff081964, 0x19272520, 0x00000000},
    {0xfd1aaf59, 0x1964c8e0, 0xfffe1330},
    {0xfb3468f9, 0x19de4780, 0xfff67c76},
    {0xf959f519, 0x1a9016a0, 0xffe5d3b0},
    {0xf78fe581, 0x1b74f940, 0xffc93c07},
    {0xf5daa371, 0x1c8614a0, 0xff9e96eb},
    {0xf43e64f1, 0x1dbb0d60, 0xff64b26c},
    {0xf2bf2271, 0x1f0a29c0, 0xff1b71b7},
    {0xf1608cc1, 0x20687a80, 0xfec3eeb7},
    {0xf0260441, 0x21ca0900, 0xfe6093dd},
    {0xef129061, 0x232208c0, 0xfdf52d35},
    {0xee28d841, 0x24631000, 0xfd86f035},
    {0xed6b1c01, 0x257f5080, 0xfd1c79d1},
    {0xecdb2fa1, 0x2668d380, 0xfcbdc271},
    {0xec7a75c1, 0x2711b940, 0xfc7407ad},
    {0xec49dce1, 0x276c7700, 0xfc49ac31}
};

Q(_t) Q(_sincos_polyval_p2)(Q(_t) _x,
                            Q(_t) _c0,
                            Q(_t) _c1,
                            Q(_t) _c2)
{
    return _c0 + Q(_mul)(_c1,_x) + Q(_mul)(_c2,Q(_mul)(_x,_x));
}

void Q(_angle_quadrant_index)(Q(_t) _theta,
                              unsigned int *_quadrant,
                              unsigned int *_index,
                              Q(_t) * _x)
{
    // Ensure theta >= 0
    // Because the qtype representation of angles is [-2*pi,2*pi] we can guarantee
    // adding 2*pi will make theta non-negative
    if ( _theta < 0 )
        _theta += Q(_2pi);

    // _theta now has the form : 0QQB BBBB BBBx xxxx xxxx xxxx xxxx xxxx
    // Q : 2-bit quadrant
    // B : 8-bit phase resolution
    // x : remaining 21 bits phase resolution, ignored

    // Extract quadrant
    //   1. Shift by 29, len(x) + len(B)
    //   2. Mask with 0x0003 to ensure 2-bit value
    *_quadrant = (_theta >> 29) & 0x0003;

    // Extract appropriate phase index:
    //   1. Shift by 25, number of ignored bits in phase
    //   2. Mask with 0x000F (15), the maximum index for lookup table
    *_index = (_theta >> 25) & 0x000F;

    // TODO : set appropriate shift amount
    // x is in [0,1)
    *_x = (_theta >> 1) & 0x0fffffff;
}

void Q(_sincos_pwpoly)(Q(_t) _theta,
                       Q(_t) * _sin,
                       Q(_t) * _cos)
{
    unsigned int quadrant;
    unsigned int sin_index;
    unsigned int cos_index;
    Q(_t) xs;
    Q(_t) xc;
    
    // polynomial coefficients
    Q(_t) c2;
    Q(_t) c1;
    Q(_t) c0;

    // Extract 2-bit quadrant and 8-bit phase index
    // Q1 | Q0
    // ---+---
    // Q2 | Q3
    Q(_angle_quadrant_index)(_theta, &quadrant, &sin_index, &xs);
    cos_index = sin_index;
    xc = xs;

#if DEBUG_SINCOS_PWPOLY
    printf("    theta : %12.8f (0x%.8x > %12.8f)\n", Q(_angle_fixed_to_float)(_theta), _theta, Q(_fixed_to_float)(_theta));
    printf("    quadrant : %u\n", quadrant);
    printf("    index    : %u\n", sin_index);
    printf("    x        : %f\n", Q(_fixed_to_float)(x));
#endif

    // sine: for quadrants Q1 & Q3 phase is inverted
    if (quadrant & 0x01) {
        xs = (1<<Q(_fracbits)) - xs;
        sin_index = 15 - sin_index;
#if DEBUG_SINCOS_PWPOLY
        printf("  sine:\n");
        printf("    x        : %f (phase inversion)\n", Q(_fixed_to_float)(xs));
        printf("    index    : %u (phase inversion)\n", sin_index);
#endif
    }

    // compute sine
    c2 = Q(_sine_table_pwpolyfit)[sin_index][0];
    c1 = Q(_sine_table_pwpolyfit)[sin_index][1];
    c0 = Q(_sine_table_pwpolyfit)[sin_index][2];
#if DEBUG_SINCOS_PWPOLY
    printf("  sine:\n");
    printf("    c0 : %12.8f\n", Q(_fixed_to_float)(c0));
    printf("    c1 : %12.8f\n", Q(_fixed_to_float)(c1));
    printf("    c2 : %12.8f\n", Q(_fixed_to_float)(c2));
#endif
    // sine: for quadrants Q2 & Q3 value is negated
    Q(_t) s = Q(_sincos_polyval_p2)(xs,c0,c1,c2);
    if (quadrant & 0x02)
        s = -s;

    // cosine: for quadrants Q0 & Q2 phase is inverted
    if (quadrant == 0 || quadrant == 2) {
        xc = (1<<Q(_fracbits)) - xc;
        cos_index = 15 - cos_index;
#if DEBUG_SINCOS_PWPOLY
        printf("  cosine:\n");
        printf("    x        : %f (phase inversion)\n", Q(_fixed_to_float)(xc));
        printf("    index    : %u (phase inversion)\n", cos_index);
#endif
    }

    // compute cosine
    c2 = Q(_sine_table_pwpolyfit)[cos_index][0];
    c1 = Q(_sine_table_pwpolyfit)[cos_index][1];
    c0 = Q(_sine_table_pwpolyfit)[cos_index][2];
#if DEBUG_SINCOS_PWPOLY
    printf("  cosine:\n");
    printf("    c0 : %12.8f\n", Q(_fixed_to_float)(c0));
    printf("    c1 : %12.8f\n", Q(_fixed_to_float)(c1));
    printf("    c2 : %12.8f\n", Q(_fixed_to_float)(c2));
#endif
    // cosine: for quadrants Q1 & Q2 value is negated
    Q(_t) c = Q(_sincos_polyval_p2)(xc,c0,c1,c2);
    if ( quadrant==1 || quadrant==2 )
        c = -c;

    *_sin = s;
    *_cos = c;
}


Q(_t) Q(_sin)(Q(_t) _theta)
{
    Q(_t) s;
    Q(_t) c;
    Q(_sincos_pwpoly)(_theta, &s, &c);
    return s;
}

Q(_t) Q(_cos)(Q(_t) _theta)
{
    Q(_t) s;
    Q(_t) c;
    Q(_sincos_pwpoly)(_theta, &s, &c);
    return c;
}

