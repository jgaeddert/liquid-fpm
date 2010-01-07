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
// Table generation for sinh|cosh CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 32;

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

    printf("// auto-generated file : do not edit\n");
    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n");
    printf("\n");

    printf("// sinh|cosh cordic coefficients: A[k] = arctanh(2^-k)\n");
    printf("const q32_t q32_sinhcosh_cordic_Ak_tab[%u] = {\n", n);
    for (k=0; k<n; k++)
        printf("    0x%.8x%s",sinhtab[k], (k<n-1) ? ",\n" : "};\n\n");

    printf("// scaling factor 1/Kp:  \n");
    printf("//   Kp = prod(k=0,infty){ sqrt(1 - 2^(-2*k)) } *\n");
    printf("//        prod(k=0,infty){ sqrt(1 - 2^(-2*i_k)) }\n");
    printf("// where\n");
    printf("//   i_0     = 4,\n");
    printf("//   i_(k+1) = 3*i_k + 1\n");
    printf("const q32_t q32_sinhcosh_cordic_kp_inv = 0x%.8x;\n\n", kp_inv);

    return 0;
}
