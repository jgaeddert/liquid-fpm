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

int main() {
    unsigned int n = 12;
    double b = 2.0;
    double x = 0.5f;

    // generate table
    double logtab[n];
    double inv_log_b = 1.0 / log(b);
    double inv_2_n   = 1.0;
    unsigned int i;
    for (i=0; i<n; i++) {
        logtab[i] = log(1.0 + inv_2_n) * inv_log_b;
        inv_2_n *= 0.5;
    }

    double tn = 0.0;
    double en = 1.0;
    double un = 0.0;
    double vn = 1.0;
    int dn;
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n", tn, en, logtab[0]);
    for (i=1; i<n; i++) {
        vn *= 0.5;
        while (1) {
            un = tn + logtab[i];
            dn = (un <= x);
            printf("%4u %12.8f %12.8f %12.8f %12.8f\n", i, un, tn, en, logtab[i]);
            if (dn==0) break;
            tn = un;
            en += en*vn;
        }
    }
    printf("true:%12.8f              %12.8f\n", x, exp2f(x));

    return 0;
}
