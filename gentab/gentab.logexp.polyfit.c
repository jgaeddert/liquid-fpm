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
// Table/constant generation for log|exp polynomials
//
// qtype_log2_polyfit:
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.logexp.polyfit usage:\n");
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

    fprintf(fid,"// auto-generated file : do not edit\n");
    fprintf(fid,"\n");
    fprintf(fid,"#include \"liquidfpm.internal.h\"\n");
    fprintf(fid,"\n");

    // 
    // qtype_log2_polyfit
    //
    float log2_poly_p0 = -1.685874806524239;
    float log2_poly_p1 =  2.028812209786359;
    float log2_poly_p2 = -0.342937403262120;
    fprintf(fid,"// %s_log2_polyfit\n",qtype);
    fprintf(fid,"const %s_t %s_log2_poly_p0 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(log2_poly_p0,intbits,fracbits));
    fprintf(fid,"const %s_t %s_log2_poly_p1 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(log2_poly_p1,intbits,fracbits));
    fprintf(fid,"const %s_t %s_log2_poly_p2 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(log2_poly_p2,intbits,fracbits));
    fprintf(fid,"\n\n");

    return 0;
}
