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
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.logexp.shiftadd usage:\n");
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

    unsigned int n=32;  // number of elements in the table
    float b=2.0f;       // base of logarithm

    // generate table: Ak = log_b( 1 + 2^-k )
    int logtab[n];
    double inv_log_b = 1.0 / log(b);
    double inv_2_n   = 1.0;
    double tabval;
    unsigned int i;
    for (i=0; i<n; i++) {
        tabval = log(1.0 + inv_2_n) * inv_log_b;
        logtab[i] = qtype_float_to_fixed(tabval,intbits,fracbits);
        inv_2_n *= 0.5;
    }

    printf("// auto-generated file : do not edit\n");
    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n");
    printf("\n");

    // Find maximum number of iterations (look for first zero
    // in the log table)
    unsigned int nmax=0;
    for (i=0; i<n; i++)
        if (logtab[i] == 0) break;
    nmax = i-1;

    printf("// Pre-computed look-up table: A[k] = log2( 1 + 2^-k )\n");
    printf("const %s_t %s_log2_shiftadd_Ak_tab[%u] = {\n", qtype,qtype,n);
    for (i=0; i<n; i++)
        printf("    0x%.8x%s",logtab[i], (i<n-1) ? ",\n" : "};\n\n");


    printf("// Maximum number of iterations, given the shiftadd_Ak_table\n");
    printf("// above.  The shift|add algorithm will hit an infinite loop\n");
    printf("// condition for values in the table equal to zero, hence this\n");
    printf("// limitation.\n");
    printf("const unsigned int %s_log2_shiftadd_nmax = %u;\n\n", qtype, nmax);

    // compute base conversion constants
    float ln2     = logf(2.0f);
    float log10_2 = log10f(2.0f);
    float log2_e  = log2f(expf(1));
    float log2_10 = log2f(10.0f);
    printf("// constants for logarithm base conversions\n");
    printf("const %s_t %s_ln2     = 0x%.8x; // log(2)\n",   qtype,qtype, qtype_float_to_fixed(ln2,intbits,fracbits));
    printf("const %s_t %s_log10_2 = 0x%.8x; // log(10)\n",  qtype,qtype, qtype_float_to_fixed(log10_2,intbits,fracbits));
    printf("const %s_t %s_log2_e  = 0x%.8x; // log2(e)\n",  qtype,qtype, qtype_float_to_fixed(log2_e,intbits,fracbits));
    printf("const %s_t %s_log2_10 = 0x%.8x; // log2(10)\n", qtype,qtype, qtype_float_to_fixed(log2_10,intbits,fracbits));
    printf("\n");

    return 0;
}
