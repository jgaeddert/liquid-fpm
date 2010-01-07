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
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 20;
    double b = 2.0;
    float xf = sqrt(2.0f);
    q32_t x = q32_float_to_fixed(xf);

    int print_table=1;

    // generate table: Ak = log_b( 1 + 2^-k )
    q32_t logtab[n];
    double inv_log_b = 1.0 / log(b);
    double inv_2_n   = 1.0;
    double tabval;
    unsigned int i;
    for (i=0; i<n; i++) {
        tabval = log(1.0 + inv_2_n) * inv_log_b;
        logtab[i] = q32_float_to_fixed(tabval);
        inv_2_n *= 0.5;
    }

    // Find maximum number of iterations (look for first zero
    // in the log table)
    unsigned int nmax=0;
    for (i=0; i<n; i++)
        if (logtab[i] == 0) break;
    nmax = i-1;

    if (print_table) {
        printf("const q32_t q32_log2_shiftadd_Ak_tab[%u] = {\n", n);
        for (i=0; i<n; i++) {
            printf("    0x%.8x%s",logtab[i], (i<n-1) ? ",\n" : "};\n\n");
        }
        printf("const q32_t q32_log2_shiftadd_nmax = %u;\n\n", nmax);
    }

    q32_t tn = 0;
    q32_t en = q32_one;
    q32_t un = 0;
    q32_t vn = q32_one;
    int dn;
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(tn),
            q32_fixed_to_float(en),
            q32_fixed_to_float(logtab[0]));
    if (n>nmax) n=nmax;
    for (i=1; i<n; i++) {
        vn >>= 1;
        while (1) {
            //un = en + q32_mul(en,vn);
            un = en;
            un += en>>i;
            dn = (un <= x);
            printf("%4u %12.8f %12.8f %12.8f %12.4e\n",
                    i,
                    q32_fixed_to_float(un),
                    q32_fixed_to_float(tn),
                    q32_fixed_to_float(en),
                    q32_fixed_to_float(logtab[i]));
            if (dn == 0) break;
            tn += logtab[i];
            en = un;
        }
    }
    printf("true:%12.8f %12.8f\n", xf, log2f(xf));

    return 0;
}
