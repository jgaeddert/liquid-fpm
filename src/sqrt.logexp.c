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
// square root approximation
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_SQRT_LOGEXP 0

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

//
// use log2 and exp2 functions to approximate square root
//
Q(_t) Q(_sqrt_logexp)( Q(_t) _x, unsigned int _n )
{
    if (_x <= 0) {
        fprintf(stderr,"error: qxx_sqrt(), x must be > 0");
        exit(1);
    }

    // TODO : ensure precision is maintained by splitting log2
    //        output into base and fractional components

    // compute logarithm
    Q(_t) log2x = Q(_log2)(_x);

    // divide by 2 (logical bit shift)
    Q(_t) log2x_by_2 = log2x >> 1;

    // compute exponential
    Q(_t) qsqrt = Q(_exp2)(log2x_by_2);

#if DEBUG_SQRT_LOGEXP
    printf("    x           = %12.10f\n", Q(_fixed_to_float)(_x));
    printf("    log2(x)     = %12.10f\n", Q(_fixed_to_float)(log2x));
    printf("    log2(x)/2   = %12.10f\n", Q(_fixed_to_float)(log2x_by_2));
    printf("    sqrt(x)     = %12.10f\n", Q(_fixed_to_float)(qsqrt));
#endif
    return qsqrt;
}

