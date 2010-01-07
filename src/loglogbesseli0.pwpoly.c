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
// piece-wise approximation to BesselI0 using polynomials
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.internal.h"

#define DEBUG_LOGLOGBESSELI0    1

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// region boundaries
extern const Q(_t) Q(_loglogbesseli0_r1);
extern const Q(_t) Q(_loglogbesseli0_r2);

// polynomials: regions 1, 2, and 3
extern Q(_t) Q(_loglogbesseli0_p1)[4];
extern Q(_t) Q(_loglogbesseli0_p2)[4];
extern Q(_t) Q(_loglogbesseli0_p3)[4];

Q(_t) Q(_loglogbesseli0)(Q(_t) _z)
{
    if (_z <= 0) {
        printf("error: _z must be greater than zero\n");
        printf("       %12.8f\n", Q(_fixed_to_float)(_z));
        exit(0);
    }

    unsigned int _n = 32;

    // compute t = log2(z)
    Q(_t) t = Q(_log2_shiftadd)(_z,_n);

    // output
    Q(_t) y;

    // pointer to polynomial array
    Q(_t) * p = NULL;
    
    // set appropriate polynomial based on region
    if (t < Q(_loglogbesseli0_r1)) {
        // region 1
        p = Q(_loglogbesseli0_p1);
    } else if (t < Q(_loglogbesseli0_r2)) {
        // region 2
        p = Q(_loglogbesseli0_p2);
    } else {
        // region 3
        p = Q(_loglogbesseli0_p3);
    }
    
    // evaluate polynomial
    // TODO : use more efficient method to evaluate polynomial
    y = Q(_mul)(Q(_mul)(Q(_mul)(p[0],t),t),t) +
                Q(_mul)(Q(_mul)(p[1],t),t) +
                        Q(_mul)(p[2],t) +
                                p[3];

    return y;
}

