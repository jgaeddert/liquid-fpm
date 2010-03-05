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

#ifndef LIQUIDFPM_INV_NEWTON_AUTOTEST_H
#define LIQUIDFPM_INV_NEWTON_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

void inv_newton_autotest()
{
    if (_autotest_verbose)
        printf("testing inv() [Newton]...\n");

    unsigned int n=32;  // precision
    unsigned int num_steps = 32;
    float xmin = 1.01f / Q(_fixed_to_float)(Q(_max));
    float xmax = Q(_fixed_to_float)(Q(_max))*0.99f;
    //float dx = (xmax - xmin)/((float)(num_steps-1));
    float sigma = powf(xmin/xmax,-1.0f/(float)(num_steps-1));
    float tol = Q(_fixed_to_float)(1<<(Q(_intbits)+2));

    // testing variables
    float xf;
    float yf;
    Q(_t) x;
    Q(_t) y;
    float ytest;

    unsigned int i;
    xf = xmin;
    for (i=0; i<num_steps; i++) {
        // compute true inverse (floating-point)
        yf = 1.0f / xf;

        // compute fixed-point inverse
        x = Q(_float_to_fixed)(xf);
        y = Q(_inv_newton)(x,n);

        ytest = Q(_fixed_to_float)(y);

        CONTEND_DELTA(yf,ytest,tol);

        if (_autotest_verbose) {
            printf("inv(%12.8f) = %12.8f (error: %12.4e)\n",
                xf,
                ytest,
                yf-ytest);
        }

        // increment input parameter
        xf *= sigma;
    }

}

#endif // LIQUIDFPM_INV_NEWTON_AUTOTEST_H
