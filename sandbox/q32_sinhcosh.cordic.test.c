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
// sinh/cosh using CORDIC (fixed-point)
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 32;
    float thetaf = 1.0f;
    q32_t theta = q32_float_to_fixed(thetaf);

    int print_table=1;

    // generate table
    q32_t sinhtab[n];
    double inv_2_n   = 1.0;
    double Ak;
    unsigned int k;
    for (k=0; k<n; k++) {
        // precompute Ak
        Ak = atanhf(inv_2_n);
        sinhtab[k] = q32_float_to_fixed(Ak);
        inv_2_n *= 0.5;
    }

    q32_t kp_inv = q32_float_to_fixed(1.2074970677630721288777210113109158368127832217698134);

    if (print_table) {
        printf("const q32_t q32_sinh_cordic_tab[%u] = {\n", n);
        for (k=0; k<n; k++)
            printf("    0x%.8x%s",sinhtab[k], (k<n-1) ? ",\n" : "};\n\n");
        printf("const q32_t q32_cordic_kp_inv = 0x%.8x;\n\n", kp_inv);
    }

    q32_t x = kp_inv;
    q32_t y = 0;
    q32_t z = theta;
    q32_t d,tx,ty,tz;
    unsigned int i=4;
    printf("   n           x            y            z         -d*An\n");
    printf("init: %12.8f %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            0.0);
    for (k=1; k<n; k++) {
        d = ( z>=0 ) ? 0 : -1;
        // d = z >> 31;
    loop1:
        tx = x + ((y>>k)^d)-d;
        ty = y + ((x>>k)^d)-d;
        tz = z - ((sinhtab[k]^d)-d);
        x = tx;
        y = ty;
        z = tz;
        printf("%5u %12.8f %12.8f %12.8f %12.8f\n",
            k,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(sinhtab[k])*(z>=0?1.0:-1.0));

        if (k==i) {
            i = 3*i+1;
            goto loop1;
        }
    }
    printf("true: %12.8f %12.8f\n",coshf(thetaf),sinhf(thetaf));

    return 0;
}
