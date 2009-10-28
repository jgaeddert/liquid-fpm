/*
 * Copyright (c) 2008, 2009 Joseph Gaeddert
 * Copyright (c) 2008, 2009 Virginia Polytechnic Institute & State University
 *
 * This file is part of liquid-fpm.
 *
 * liquid-fpm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid-fpm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid-fpm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIQUIDFPM_SINCOS_CORDIC_AUTOTEST_H
#define LIQUIDFPM_SINCOS_CORDIC_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

void sincos_cordic_autotest()
{
    unsigned int n=32;  // precision
    unsigned int num_steps = 37;
    float xmin = -2*M_PI;
    float xmax =  2*M_PI;
    float dx = (xmax - xmin)/((float)(num_steps-1));
    float tol = 1e-4f;

    // testing variables
    float xf;
    float cf, sf;
    q32_t x;
    q32_t c, s;
    float ctest, stest;

    unsigned int i;
    xf = xmin;
    for (i=0; i<num_steps; i++) {
        // compute true inverse (floating-point)
        cf = cosf(xf);
        sf = sinf(xf);

        // compute fixed-point value
        x = Q(_angle_float_to_fixed)(xf);
        Q(_sincos_cordic)(x,&s,&c,n);

        ctest = Q(_fixed_to_float)(c);
        stest = Q(_fixed_to_float)(s);

        // run the comparison(s)
        CONTEND_DELTA(cf,ctest,tol);
        CONTEND_DELTA(sf,stest,tol);

        if (_autotest_verbose) {
            printf("sin|cos(%12.8f) = %12.8f (%12.8f) | %12.8f (%12.8f)\n",
                    xf,
                    stest,
                    sf,
                    ctest,
                    cf);
        }

        // increment input parameter
        xf += dx;
    }

}

#endif // LIQUIDFPM_SINCOS_CORDIC_AUTOTEST_H

