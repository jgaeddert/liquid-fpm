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
//
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// polynomial coefficients
extern const Q(_t) Q(_log2_poly_p0);
extern const Q(_t) Q(_log2_poly_p1);
extern const Q(_t) Q(_log2_poly_p2);

#define DEBUG_LOG2_POLY 0

//
// use second-order polynomial fit
//
Q(_t) Q(_log2)( Q(_t) _x )
{
    if (_x <= 0) {
        fprintf(stderr,"error: qxx_log2(), x must be > 0");
        exit(1);
    }

    // base index
    int b = liquidfpm_msb_index(_x) - 1;

    // compute fractional portion
    Q(_t) x = _x ^ (1<<b);  // clear most significant bit
    Q(_t) f = (b > Q(_fracbits)) ? x >> (b-Q(_fracbits)) : x << (Q(_fracbits)-b);
    f += 1<<Q(_fracbits);

#if DEBUG_LOG2_POLY
    printf("    f = %12.8f\n", Q(_fixed_to_float)(f));
#endif
    Q(_t) x2 = Q(_mul)(f,f);
#if DEBUG_LOG2_POLY
    printf("    x2= %12.8f\n", Q(_fixed_to_float)(x2));
#endif

    Q(_t) v0    = Q(_log2_poly_p0);
    Q(_t) v1    = Q(_mul)(Q(_log2_poly_p1), f);
    Q(_t) v2    = Q(_mul)(Q(_log2_poly_p2), x2);
#if DEBUG_LOG2_POLY
    printf("    v0 = %12.8f\n", Q(_fixed_to_float)(v0));
    printf("    v1 = %12.8f\n", Q(_fixed_to_float)(v1));
    printf("    v2 = %12.8f\n", Q(_fixed_to_float)(v2));
#endif

    Q(_t) polyval = v0 + v1 + v2;
#if DEBUG_LOG2_POLY
    printf("    p = %12.8f\n", Q(_fixed_to_float)(polyval));
    printf("    log2(%f) = %d + %12.10f\n", Q(_fixed_to_float)(_x), b-Q(_fracbits), Q(_fixed_to_float)(polyval));
#endif

    b -= Q(_fracbits);
    return polyval + ((b<0) ? -(abs(b)<<Q(_fracbits)) : (b<<Q(_fracbits)) );
}

