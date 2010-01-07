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
// ratio : compute rational numbers with high precision
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_RATIO 0

#define Q(name)         LIQUIDFPM_CONCAT(q32,name)

// compute ratio
Q(_t) Q(_ratio)(int _a, int _b, unsigned int _n)
{
    if (_b == 0) {
        fprintf(stderr,"error: qtype_ratio(), divide by zero\n");
        exit(1);
    } else if ( abs(_a) > abs(_b) ) {
        fprintf(stderr,"warning: qtype_ratio(), numerator > denominator; overflow possible\n");
    }

    if (_a == 0) {
        return 0;
    }

    int negate = ((_a<0)&&(_b>0)) || ((_a>0)&&(_b<0));
    _a = abs(_a);
    _b = abs(_b);

    int base_a = msb_index(_a) - 1;
    int shift_a = (int)(Q(_fracbits)) - base_a;
    Q(_t) a_hat = shift_a > 0 ? _a<<shift_a : _a>>(-shift_a);

    int base_b = msb_index(_b) - 1;
    int shift_b = (int)(Q(_fracbits)) - base_b;
    Q(_t) b_hat = shift_b > 0 ? _b<<shift_b : _b>>(-shift_b);

    Q(_t) ratio = Q(_div_inv_newton)(a_hat, b_hat, _n);

    int shift = shift_b - shift_a;
    ratio = shift > 0 ? ratio<<shift : ratio>>(-shift);

#if DEBUG_RATIO
    printf("|a| : %6u (%12.8f * 2^%4d)\n", abs(_a), q32_fixed_to_float(a_hat), q32_fracbits-shift_a);
    printf("|b| : %6u (%12.8f * 2^%4d)\n", abs(_b), q32_fixed_to_float(b_hat), q32_fracbits-shift_b);
#endif

    return negate ? -ratio : ratio;
}

