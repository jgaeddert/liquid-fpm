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

#ifndef LIQUIDFPM_ATAN2_CORDIC_AUTOTEST_H
#define LIQUIDFPM_ATAN2_CORDIC_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

void atan2_cordic_autotest()
{
    unsigned int n=32;  // precision
    unsigned int num_steps = 37;
    float theta_min = -2*M_PI;
    float theta_max =  2*M_PI;
    float dtheta = (theta_max - theta_min)/((float)(num_steps-1));
    float rmin      =  0.1f;
    float rmax      =  1.0f;
    float dr = (rmax - rmin)/((float)(num_steps-1));
    float tol = 1e-4f;

    // testing variables
    float rf, thetaf, thetaf_unwrapped;
    float xf, yf;
    q32_t r, theta;
    q32_t c, s;
    float r_test, theta_test;

    unsigned int i;
    thetaf = theta_min;
    rf = rmin;
    for (i=0; i<num_steps; i++) {
        // compute "true" (floating-point) value(s)
        xf = rf*cosf(thetaf);
        yf = rf*sinf(thetaf);

        // compute fixed-point value(s)
        c = Q(_float_to_fixed)(xf);
        s = Q(_float_to_fixed)(yf);
        Q(_atan2_cordic)(s,c,&r,&theta,n);

        // unwrap phase terms
        thetaf_unwrapped = thetaf;
        while (thetaf_unwrapped < -M_PI) thetaf_unwrapped += 2*M_PI;
        while (thetaf_unwrapped >  M_PI) thetaf_unwrapped -= 2*M_PI;
        if (fabsf(thetaf_unwrapped) > M_PI/2.0f) {
            // shift angle to be in [pi/2, 3*pi/2]
            if (theta < 0) theta += Q(_2pi);
            if (thetaf_unwrapped < 0.0f) thetaf_unwrapped += 2*M_PI;
        }

        // compute floating-point test value(s)
        r_test = Q(_fixed_to_float)(r);
        theta_test = Q(_angle_fixed_to_float)(theta);

        // run the comparison(s)
        CONTEND_DELTA(rf,r_test,tol);
        CONTEND_DELTA(thetaf_unwrapped,theta_test,tol);

        if (_autotest_verbose) {
            printf("(%12.8f,%12.8f): r|atan2 = %12.8f (%12.8f) | %12.8f (%12.8f)\n",
                    yf, xf,
                    r_test, rf,
                    theta_test, thetaf_unwrapped);
        }

        // increment input parameter(s)
        thetaf += dtheta;
        rf     += dr;
    }

}

#endif // LIQUIDFPM_ATAN2_CORDIC_AUTOTEST_H

