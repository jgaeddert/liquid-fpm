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
// gentab.logsin.c
//
// Table/constant look-up table for sin, log2
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.logsin usage:\n");
    printf("  u/h   :   print this help file\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    //printf("    o   :   output filename [default: standard output]\n");
}

int main(int argc, char * argv[]) {
    // initialize variables, set defaults
    FILE * fid = stdout;
    char qtype[64] = "q32";
    unsigned int intbits=7;
    unsigned int fracbits=25;

    unsigned int sine_tabsize = 256;
    unsigned int log2_tabsize = 256;

    unsigned int values_per_line=4;

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
    unsigned int n = intbits + fracbits;
    if (n != 8 && n != 16 && n != 32) {
        fprintf(stderr,"error: %s, invalid total bits (%u), must be 8,16,32\n", argv[0], n);
        exit(-1);
    }

    unsigned int i;

    // generate header

    fprintf(fid,"// auto-generated file (do not edit)\n");
    fprintf(fid,"// invoked as : ");
    for (i=0; i<argc; i++)
        fprintf(fid,"%s ", argv[i]);
    fprintf(fid,"\n\n");

    fprintf(fid,"#include \"liquidfpm.internal.h\"\n\n");

    // generate sine table
    fprintf(fid,"// sine table\n");
    fprintf(fid,"const %s_t %s_sin_table[%u] = {\n    ", qtype,qtype,sine_tabsize);
    for (i=0; i<sine_tabsize; i++) {
        float sine = sin( (M_PI/2.0) * ((double)i) / (double)(sine_tabsize-1));
        //printf("%4u: %12.8f\n",i,sine);

        fprintf(fid,"0x%.8x", qtype_float_to_fixed(sine,intbits,fracbits));
        if ( i == (sine_tabsize-1) )
            fprintf(fid,"\n};\n");
        else if ( ((i+1)%values_per_line) == 0 )
            fprintf(fid,",\n    ");
        else
            fprintf(fid,", ");
    }

    // generate log2 fraction table
    fprintf(fid,"\n");
    fprintf(fid,"// log2 fraction table\n");
    fprintf(fid,"const %s_t %s_log2_fraction_table[%d] = {\n    ", qtype,qtype,log2_tabsize);
    for (i=0; i<log2_tabsize; i++) {
        double log2val = log2( 1.0 + ((double)i)/((double)log2_tabsize) );

        fprintf(fid,"0x%.8x", qtype_float_to_fixed(log2val,intbits,fracbits));
        if ( i == (log2_tabsize-1) )
            fprintf(fid,"\n};\n");
        else if ( ((i+1)%values_per_line) == 0 )
            fprintf(fid,",\n    ");
        else
            fprintf(fid,", ");
    }

    fprintf(fid,"\n\n");

    return 0;
}

