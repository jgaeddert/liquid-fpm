//
// gentab_math.c
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

#include "../include/liquidfpm.h"

int main(int argc, char * argv[]) {

    // initialize variables, set defaults
    unsigned int fracbits=28;
    unsigned int intbits=4;
    unsigned int tabsize=256;

    unsigned int i;
    unsigned int n=8;

    // generate header

    printf("// auto-generated file (do not edit)\n\n");

    printf("\n");
    printf("#include \"liquidfpm.internal.h\"\n\n");

    // generate sine table
    printf("const q32_t sin_table_q32[%u] = {\n    ", tabsize);
    for (i=0; i<tabsize; i++) {
        float sine = sin( (M_PI/2.0) * ((double)i) / (double)(tabsize-1));
        //printf("%4u: %12.8f\n",i,sine);

        printf("0x%.8x", q32_float_to_fixed(sine));
        if ( i == (tabsize-1) )
            printf("\n};\n");
        else if ( ((i+1)%n) == 0 )
            printf(",\n    ");
        else
            printf(", ");
    }

    unsigned int log2_tabsize = 256;
    printf("const unsigned int log2_fraction_table[%d] = {\n    ", log2_tabsize);
    for (i=0; i<log2_tabsize; i++) {
        double log2val = log2( 1.0 + ((double)i)/((double)log2_tabsize) );
        unsigned int frac_val = (unsigned int) (log2_tabsize*log2val);

        printf("%3u", frac_val);
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

