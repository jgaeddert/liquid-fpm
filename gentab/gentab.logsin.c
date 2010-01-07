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
// gentab_math.c
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/liquidfpm.h"

int main(int argc, char * argv[]) {

    // initialize variables, set defaults
    //unsigned int fracbits=28;
    //unsigned int intbits=4;
    unsigned int sine_tabsize=256;
    unsigned int log2_tabsize = 256;

    unsigned int i;
    unsigned int n=8;

    // generate header

    printf("// auto-generated file (do not edit)\n\n");

    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n\n");

    // generate sine table
    printf("// sine table\n");
    printf("const q32_t q32_sin_table[%u] = {\n    ", sine_tabsize);
    for (i=0; i<sine_tabsize; i++) {
        float sine = sin( (M_PI/2.0) * ((double)i) / (double)(sine_tabsize-1));
        //printf("%4u: %12.8f\n",i,sine);

        printf("0x%.8x", q32_float_to_fixed(sine));
        if ( i == (sine_tabsize-1) )
            printf("\n};\n");
        else if ( ((i+1)%n) == 0 )
            printf(",\n    ");
        else
            printf(", ");
    }

    // generate log2 fraction table
    printf("\n");
    printf("// log2 fraction table\n");
    printf("const q32_t q32_log2_fraction_table[%d] = {\n    ", log2_tabsize);
    for (i=0; i<log2_tabsize; i++) {
        double log2val = log2( 1.0 + ((double)i)/((double)log2_tabsize) );
        q32_t frac_val = q32_float_to_fixed(log2val);

        printf("0x%.8x", frac_val);
        if ( i == (log2_tabsize-1) )
            printf("\n};\n");
        else if ( ((i+1)%n) == 0 )
            printf(",\n    ");
        else
            printf(", ");
    }

    printf("\n\n");

    return 0;
}

