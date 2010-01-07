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
// sinh/cosh using CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned int n = 16;
    double theta = 1.0f;

    // generate table
    double sinhtab[n];
    double inv_2_n   = 1.0;
    unsigned int k;
    for (k=0; k<n; k++) {
        sinhtab[k] = atanh(inv_2_n);
        inv_2_n *= 0.5;
    }

    double kp_inv = 1.2074970677630721288777210113109158368;

    double x = kp_inv;
    double y = 0.0;
    double z = theta;
    double v = 1.0;
    double d,tx,ty,tz;
    unsigned int i=4;
    printf("   n           x            y            z         -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n", x,y,z,0.0);
    for (k=1; k<n; k++) {
        v *= 0.5f;
loop1:
        d = ( z>=0.0 ) ? +1.0 : -1.0;
        tx = x + d * v * y;
        ty = y + d * v * x;
        tz = z - d * sinhtab[k];
        x = tx;
        y = ty;
        z = tz;
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n", k, x, y, z, d*sinhtab[k]);
        if (k==i) {
            i = 3*i+1;
            goto loop1;
        }

    }

    printf("true:%12.8f %12.8f\n",cosh(theta),sinh(theta));
    // c = x;
    // s = y;
    return 0;
}
