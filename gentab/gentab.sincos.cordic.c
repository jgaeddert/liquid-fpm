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
// Table generation for sin|cos CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.sincos.cordic usage:\n");
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

    unsigned int i;
    unsigned int n = 32;

    fprintf(fid,"// auto-generated file : do not edit\n");
    fprintf(fid,"// invoked as : ");
    for (i=0; i<argc; i++)
        fprintf(fid,"%s ", argv[i]);
    fprintf(fid,"\n\n");

    fprintf(fid,"#include \"liquidfpm.internal.h\"\n");
    fprintf(fid,"\n");

    // 1/K scaling factor
    float k_inv = 0.60725293500888125616;

    fprintf(fid,"// scaling factor 1/K:  K = prod(k=0,infty){ sqrt(1 + 2^(-2k)) }\n");
    fprintf(fid,"const %s_t %s_cordic_k_inv = 0x%.8x;\n\n", qtype,qtype,
            qtype_float_to_fixed(k_inv,intbits,fracbits));

    // generate table
    double inv_2_n   = 1.0;
    double Ak;
    float qtype_angle_scalar = M_PI / (float)(1<<(intbits-2));
    fprintf(fid,"// cordic coefficients: A[k] = arctan(2^-k)\n");
    fprintf(fid,"const %s_t %s_cordic_Ak_tab[%u] = {\n", qtype,qtype,n);
    for (i=0; i<n; i++) {
        // precompute Ak, normalizing by angular scaling factor. This
        // is necessary by the nature of how angles are stored in
        // fixed-point decimal:
        //    2*pi  :   0x7fffffff (the largest positive number)
        //   -2*pi  :   0xffffffff (the largest negative number)
        Ak = atanf(inv_2_n) / qtype_angle_scalar;

        // write output
        fprintf(fid,"    0x%.8x%s",
                qtype_float_to_fixed(Ak,intbits,fracbits),
                (i<n-1) ? ",\n" : "};\n\n");

        // update 1 / 2^n
        inv_2_n *= 0.5;
    }

    return 0;
}

