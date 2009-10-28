//
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n=32;  // number of elements in the table
    float b=2.0f;       // base of logarithm

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
    printf("const q32_t q32_log2_shiftadd_Ak_tab[%u] = {\n", n);
    for (i=0; i<n; i++)
        printf("    0x%.8x%s",logtab[i], (i<n-1) ? ",\n" : "};\n\n");


    printf("// Maximum number of iterations, given the shiftadd_Ak_table\n");
    printf("// above.  The shift|add algorithm will hit an infinite loop\n");
    printf("// condition for values in the table equal to zero, hence this\n");
    printf("// limitation.\n");
    printf("const unsigned int q32_log2_shiftadd_nmax = %u;\n\n", nmax);

    // compute base conversion constants
    float ln2     = logf(2.0f);
    float log10_2 = log10f(2.0f);
    float log2_e  = log2f(expf(1));
    float log2_10 = log2f(10.0f);
    printf("// constants for logarithm base conversions\n");
    printf("const q32_t q32_ln2     = 0x%.8x; // log(2)\n",     q32_float_to_fixed(ln2));
    printf("const q32_t q32_log10_2 = 0x%.8x; // log(10)\n",    q32_float_to_fixed(log10_2));
    printf("const q32_t q32_log2_e  = 0x%.8x; // log2(e)\n",    q32_float_to_fixed(log2_e));
    printf("const q32_t q32_log2_10 = 0x%.8x; // log2(10)\n",   q32_float_to_fixed(log2_10));
    printf("\n");

    return 0;
}
