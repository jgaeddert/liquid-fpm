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

// polynomial coefficients
#define LIQUIDFPM_q32_exp2_poly_p2  q32_float_to_fixed( 0.343936792328645 )
#define LIQUIDFPM_q32_exp2_poly_p1  q32_float_to_fixed( 0.656063207671355 )
#define LIQUIDFPM_q32_exp2_poly_p0  q32_float_to_fixed( 1.000000000000000 )

#define DEBUG_EXP2_POLY 0

//
// use second-order polynomial fit
//
q32_t q32_exp2( q32_t _x )
{
    // base index
    int b = q32_intpart(_x);
    q32_t f = q32_fracpart(_x);

    q32_t e = 1<<(b+q32_fracbits);

    // compute fractional portion
    q32_t f2 = q32_mul(f,f);    // f^2

#if DEBUG_EXP2_POLY
    printf("    f = %12.8f\n", q32_fixed_to_float(f));
    printf("    f2= %12.8f\n", q32_fixed_to_float(f2));
#endif

    q32_t v0    = LIQUIDFPM_q32_exp2_poly_p0;
    q32_t v1    = q32_mul(LIQUIDFPM_q32_exp2_poly_p1, f);
    q32_t v2    = q32_mul(LIQUIDFPM_q32_exp2_poly_p2, f2);
#if DEBUG_EXP2_POLY
    printf("    v0 = %12.8f\n", q32_fixed_to_float(v0));
    printf("    v1 = %12.8f\n", q32_fixed_to_float(v1));
    printf("    v2 = %12.8f\n", q32_fixed_to_float(v2));
#endif

    q32_t polyval = v0 + v1 + v2;
#if DEBUG_EXP2_POLY
    printf("    p = %12.8f\n", q32_fixed_to_float(polyval));
#endif

    q32_t retval = q32_mul(e,polyval);
#if DEBUG_EXP2_POLY
    printf("    exp2(%f) = %12.10f * %12.10f = %12.8f\n",
            q32_fixed_to_float(_x),
            q32_fixed_to_float(e),
            q32_fixed_to_float(polyval),
            q32_fixed_to_float(retval));
#endif

    return retval;
}

