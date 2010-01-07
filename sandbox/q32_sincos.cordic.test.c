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
// sin/cos using CORDIC (fixed-point)
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 32;
    float thetaf = M_PI / 2.0;
    q32_t theta = q32_angle_float_to_fixed(thetaf);

    int print_table=0;

    // generate table
    q32_t sintab[n];
    double inv_2_n   = 1.0;
    double Ak;
    unsigned int i;
    for (i=0; i<n; i++) {
        // precompute Ak, normalizing by angular scaling factor. This
        // is necessary by the nature of how angles are stored in
        // fixed-point decimal:
        //    2*pi  :   0x7fffffff (the largest positive number)
        //   -2*pi  :   0xffffffff (the largest negative number)
        Ak = atanf(inv_2_n) / q32_angle_scalar;
        sintab[i] = q32_float_to_fixed(Ak);
        inv_2_n *= 0.5;
    }

    q32_t k_inv = q32_float_to_fixed(0.6072529350088812561694467525049282631123908521500897724);

    if (print_table) {
        printf("const q32_t q32_sine_cordic_tab[%u] = {\n", n);
        for (i=0; i<n; i++)
            printf("    0x%.8x%s",sintab[i], (i<n-1) ? ",\n" : "};\n\n");
        printf("const q32_t q32_cordic_k_inv = 0x%.8x;\n\n", k_inv);
    }

    q32_t x = k_inv;
    q32_t y = 0;
    q32_t z = theta;
    q32_t d,tx,ty,tz;
    printf("   n           x            y            z         -d*An\n");
    printf("init: %12.8f %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            0.0);
    for (i=0; i<n; i++) {
        d = ( z>=0 ) ? 0 : -1;
        // d = z >> 31;

        tx = x - ((y>>i)^d)-d;
        ty = y + ((x>>i)^d)-d;
        tz = z - ((sintab[i]^d)-d);
        x = tx;
        y = ty;
        z = tz;
        printf("%5u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(sintab[i])*(z>=0?1.0:-1.0));
    }
    printf("true: %12.8f %12.8f\n",cosf(thetaf),sinf(thetaf));

    return 0;
}
