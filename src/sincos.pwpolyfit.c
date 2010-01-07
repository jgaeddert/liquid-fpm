/*
 * Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
 * Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
 *                                Institute & State University
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

// 
// sin/cos piecewise poly
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SINCOS_PWPOLY 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// polynomial coefficients
extern Q(_t) Q(_sine_pwpoly_tab)[16][3];

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

    // mask out quadrature bits:
    // _x   :   000B BBBB BBBB BBBB BBBB BBBB BBBB BBBB
    *_x = _theta & 0x1fffffff;

    // compute shift amount such that _x is in [0,1) for
    // qtype format
    //
    // NOTE: this shift amount is a constant for the qtype
    //       data format
    int s = Q(_bits) - 3 - Q(_fracbits);
    *_x = s > 0 ? *_x >> s :    // shift right
                  *_x << (-s);  // shift left
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
    printf("    x        : %f\n", Q(_fixed_to_float)(xc));
#endif

    // sine: for quadrants Q1 & Q3 phase is inverted
    if (quadrant & 0x01) {
        xs = (1<<Q(_fracbits)) - xs;
        sin_index = 15 - sin_index;
#if DEBUG_SINCOS_PWPOLY
        printf("  sine:\n");
        printf("    x[sin]   : %f (phase inversion)\n", Q(_fixed_to_float)(xs));
        printf("    index    : %u (phase inversion)\n", sin_index);
#endif
    }

    // compute sine
    c2 = Q(_sine_pwpoly_tab)[sin_index][0];
    c1 = Q(_sine_pwpoly_tab)[sin_index][1];
    c0 = Q(_sine_pwpoly_tab)[sin_index][2];
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
        printf("    x[cos]   : %f (phase inversion)\n", Q(_fixed_to_float)(xc));
        printf("    index    : %u (phase inversion)\n", cos_index);
#endif
    }

    // compute cosine
    c2 = Q(_sine_pwpoly_tab)[cos_index][0];
    c1 = Q(_sine_pwpoly_tab)[cos_index][1];
    c0 = Q(_sine_pwpoly_tab)[cos_index][2];
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

