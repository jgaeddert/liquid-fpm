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
// Table generation for CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 32;

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

    printf("// auto-generated file : do not edit\n");
    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n");
    printf("\n");

    printf("// cordic coefficients: A[k] = arctan(2^-k)\n");
    printf("const q32_t q32_cordic_Ak_tab[%u] = {\n", n);
    for (i=0; i<n; i++)
        printf("    0x%.8x%s",sintab[i], (i<n-1) ? ",\n" : "};\n\n");

    printf("// scaling factor 1/K:  K = prod(k=0,infty){ sqrt(1 + 2^(-2k)) }\n");
    printf("const q32_t q32_cordic_k_inv = 0x%.8x;\n\n", k_inv);

    return 0;
}
