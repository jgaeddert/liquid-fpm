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
// atan piecewise poly
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_ATAN_PWPOLY 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

Q(_t) Q(_atan_polyval_p2)(Q(_t) _x,
                          Q(_t) _c0,
                          Q(_t) _c1,
                          Q(_t) _c2)
{
    return _c0 + Q(_mul)(_c1,_x) + Q(_mul)(_x,Q(_mul)(_c2,_x));
}

Q(_t) Q(_atan2)( Q(_t) _y, Q(_t) _x )
{
    // For all practical purposes we are observing r = y/x so the decimal
    // place in fixed point math can be ignored and we can treat y and x
    // as integers so long as we don't lose resolution

    // negate calculated angle if _y < 0
    int negate = (_y < 0);

    // subtract calculated angle from pi if _x < 0
    int sub_from_pi = (_x < 0);

    // subtract calculated angle from pi/2 if abs(y/x) < 1, or invert
    // y/x to by x/y so that log2 of ratio is positive
    int invert = 0;

    // take absolute value as we are only interested in absolute ratio
    // at this point.
    _y = abs(_y);
    _x = abs(_x);

    // compare x, y; place larger value in y, smaller in x, thus ratio
    // r = y/x is always greater than or equal to 1; keeps log2(y/x)
    // positive
    if (_y < _x) {
        // swap _x and _y values
        Q(_t) tmp = _x;
        _x = _y;
        _y = tmp;
        invert = 1;
    }

    // Ultimately we want to calculate log2(abs(y/x)).  The following
    // block of code achieves this with simple bit shifts, comparisons
    // and a 32-value lookup table.

    // TODO : explain what is going on here to compute log2(abs(y/x))

    // Extract location of MSB for _y and _x; keep values in register as
    // operations on them will be computed very quickly

    // scale values by max msb_index
    unsigned int msb_index_x = liquidfpm_msb_index(_x);
    unsigned int msb_index_y = liquidfpm_msb_index(_y);

#if DEBUG_ATAN_PWPOLY
    printf("msb_index_x: %u\n", msb_index_x);
    printf("msb_index_y: %u\n", msb_index_y);
#endif
    if (abs(msb_index_x - msb_index_y) > Q(_fracbits)) {
        // use high ratio approximation
#if DEBUG_ATAN_PWPOLY
        printf("high ratio approximation\n");
#endif

        // four conditions exist
        if (invert) {
            return (sub_from_pi) ? Q(_pi) : 0;
        } else {
            return (negate) ? -Q(_pi_by_2) : Q(_pi_by_2);
        }
    }
    unsigned int shift = 31 - ( (msb_index_x > msb_index_y) ? msb_index_x : msb_index_y );
    _x <<= shift;
    _y <<= shift;
        
#if DEBUG_ATAN_PWPOLY
    float yf = Q(_fixed_to_float)(_y);
    float xf = Q(_fixed_to_float)(_x);
    printf("  y > %12.8f, log2(y) : %12.8f\n", yf, log2(yf));
    printf("  x > %12.8f, log2(x) : %12.8f\n", xf, log2(xf));
    printf("  true log2diff : %12.8f\n", log2(yf) - log2(xf));
#endif
    Q(_t) log2diff = Q(_log2)(_y) - Q(_log2)(_x);

    unsigned int index = (log2diff >> 27) & 0x000f;
#if DEBUG_ATAN_PWPOLY
    //printf("  log2diff : %f > %d (index=%u)\n", Q(_fixed_to_float)(log2diff), Q(_intpart)(log2diff), index);
    printf("  log2diff : %12.8f\n", Q(_fixed_to_float)(log2diff));
    printf("  log2diff : 0x%.8x\n", log2diff);
#endif

    // if negative, overflow (use high ratio approximation)
    if (log2diff < 0) { return 0; };

    Q(_t) g = Q(_inv_newton)(log2diff + Q(_one), 32);
#if DEBUG_ATAN_PWPOLY
    printf("    g = %12.8f\n", Q(_fixed_to_float)(g));
#endif

    // g is in (0,1], determine which of 16 regions g lies
    if (g <= 0 || g > Q(_one)) {
        fprintf(stderr,"error: atan2_pwpolyfit(), g not in (0,1]\n");
        exit(1);
    }

    // Q(_t)    :   siiiiiiffffff....fff    s(sign), i(int), f(frac)
    // Q(_one)  :   00000001111111111111
    // g        :   0000000gggg.........
    // iiii : index
    index = (g >> (Q(_fracbits)-4)) & 0x000f;
#if DEBUG_ATAN_PWPOLY
    printf("    g : 0x%.8x (index : %u)\n", g, index );
#endif

    //index = 5;
    Q(_t) c0 = Q(_atan2_pwpoly_tab)[index][2];
    Q(_t) c1 = Q(_atan2_pwpoly_tab)[index][1];
    Q(_t) c2 = Q(_atan2_pwpoly_tab)[index][0];
#if DEBUG_ATAN_PWPOLY
    printf("    c0 : %12.8f\n", Q(_fixed_to_float)(c0));
    printf("    c1 : %12.8f\n", Q(_fixed_to_float)(c1));
    printf("    c2 : %12.8f\n", Q(_fixed_to_float)(c2));
#endif

    Q(_t) phi = Q(_atan_polyval_p2)(g,c0,c1,c2);

#if DEBUG_ATAN_PWPOLY
    printf("  polyval   : %f\n", Q(_fixed_to_float)(phi));
#endif
    // TODO : validate this shift amount
    phi <<= Q(_intbits)-3;
#if DEBUG_ATAN_PWPOLY
    printf("  angle     : %f\n", Q(_angle_fixed_to_float)(phi));
#endif

    // invert if necessary
    if (invert) phi = Q(_pi_by_2) - phi;

    // if in quadrants 3, 4 subtract from pi
    if (sub_from_pi) phi = Q(_pi) - phi;

    // negate if necessary
    if (negate) phi = -phi;

    return phi;
}

