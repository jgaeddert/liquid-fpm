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

#ifndef LIQUIDFPM_LOG2_SHIFTADD_AUTOTEST_H
#define LIQUIDFPM_LOG2_SHIFTADD_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

void log2_shiftadd_autotest()
{
    unsigned int n=32;  // precision
    unsigned int num_steps = 37;
    float xmin = Q(_fixed_to_float)( Q(_min) ); // min input
    float xmax = Q(_fixed_to_float)( Q(_max) ); // max input
    float ymin = Q(_fixed_to_float)(-Q(_max) ); // min output
    //float ymax = Q(_fixed_to_float)( Q(_max) ); // max output
    
    // adjust input accordingly
    if (xmin < exp2f(ymin)) xmin = exp2f(ymin);
    //if (xmax > exp2f(ymax)) xmax = exp2f(ymax);

    //float dx = (xmax - xmin)/((float)(num_steps-1));
    float sigma = powf(xmin/xmax,-(1.0f - 1e-6f)/(float)(num_steps-1));
    float tol = expf(-sqrtf(Q(_fracbits)));

    // testing variables
    float xf;
    float yf;
    Q(_t) x;
    Q(_t) y;
    float ytest;

    unsigned int i;
    xf = xmin;
    for (i=0; i<num_steps; i++) {
        if (xf > Q(_fixed_to_float)(Q(_max)))
            continue;

        // compute fixed-point output
        x = Q(_float_to_fixed)(xf);
        y = Q(_log2_shiftadd)(x,n);

        // compute true output (floating-point, converted
        // from fixed-point input)
        yf = log2f( Q(_fixed_to_float)(x) );

        ytest = Q(_fixed_to_float)(y);

        // run the comparison(s)
        CONTEND_DELTA(yf,ytest,tol);

        if (_autotest_verbose) {
            printf("log2(%12.8f) = %12.8f (%12.8f)\n",
                    xf,
                    ytest,
                    yf);
        }

        // increment input parameter
        //xf += dx;
        xf *= sigma;
    }

}

#endif // LIQUIDFPM_LOG2_SHIFTADD_AUTOTEST_H

