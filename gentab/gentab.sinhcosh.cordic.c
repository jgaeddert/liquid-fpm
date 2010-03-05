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
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.sinhcosh.cordic usage:\n");
    printf("  u/h   :   print this help file\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    //printf("    o   :   output filename [default: standard output]\n");
}

int main(int argc, char*argv[]) {
    // options
    FILE * fid = stdout;
    char qtype[64] = "q32";
    unsigned int intbits = 7;
    unsigned int fracbits = 25;
    unsigned int n = 32;

    // read options
    int dopt;
    while ((dopt = getopt(argc,argv,"uhn:i:f:")) != EOF) {
        switch (dopt) {
        case 'u':
        case 'h':   usage();                    return 0;
        case 'n':   strncpy(qtype,optarg,64);   break;
        case 'i':   intbits = atoi(optarg);     break;
        case 'f':   fracbits = atoi(optarg);    break;
        default:
            fprintf(stderr,"error: %s, unknown option\n", argv[0]);
            usage();
            return 1;
        }
    }

    // validate length
    unsigned int totalbits = intbits + fracbits;
    if (totalbits != 8 && totalbits != 16 && totalbits != 32) {
        fprintf(stderr,"error: %s, invalid total bits (%u), must be 8,16,32\n", argv[0], totalbits);
        exit(-1);
    }

    printf("// auto-generated file : do not edit\n");
    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n");
    printf("\n");

    float kp_inv = 1.207497067763072128878;

    printf("// scaling factor 1/Kp:  \n");
    printf("//   Kp = prod(k=0,infty){ sqrt(1 - 2^(-2*k)) } *\n");
    printf("//        prod(k=0,infty){ sqrt(1 - 2^(-2*i_k)) }\n");
    printf("// where\n");
    printf("//   i_0     = 4,\n");
    printf("//   i_(k+1) = 3*i_k + 1\n");
    printf("const %s_t %s_sinhcosh_cordic_kp_inv = 0x%.8x;\n\n", 
            qtype,qtype,
            qtype_float_to_fixed(kp_inv,intbits,fracbits));

    // generate table
    printf("// sinh|cosh cordic coefficients: A[k] = arctanh(2^-k)\n");
    printf("const %s_t %s_sinhcosh_cordic_Ak_tab[%u] = {\n", qtype,qtype,n);
    double inv_2_n   = 1.0;
    double Ak;
    unsigned int k;
    for (k=0; k<n; k++) {
        // precompute Ak (first value is ignored in algorithm regardless)
        Ak = k == 0 ? 0 : atanhf(inv_2_n);

        // write outpt value
        printf("    0x%.8x%s",
                qtype_float_to_fixed(Ak,intbits,fracbits),
                (k<n-1) ? ",\n" : "};\n\n");

        // update 1 / 2^n
        inv_2_n *= 0.5;
    }

    return 0;
}
