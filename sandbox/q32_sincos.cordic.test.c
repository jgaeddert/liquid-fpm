//
// sin/cos using CORDIC (fixed-point)
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 32;
    float thetaf = M_PI / 2.0;
    q32_t theta = q32_float_to_fixed(thetaf);

    int print_table=0;

    // generate table
    q32_t sintab[n];
    double inv_2_n   = 1.0;
    unsigned int i;
    for (i=0; i<n; i++) {
        sintab[i] = q32_float_to_fixed(atanf(inv_2_n));
        inv_2_n *= 0.5;
    }

    q32_t k_inv = q32_float_to_fixed(0.6072529350088812561694467525049282631123908521500897724);

    if (print_table) {
        printf("const q32_t q32_sine_cordic_tab[%u] = {\n", n);
        for (i=0; i<n; i++)
            printf("    0x%.8x%s",sintab[i], (i<n-1) ? ",\n" : "};\n\n");
        printf("const q32_t q32_cordic_k_inv = 0x%.8x;\n\n", k_inv);
    }

    q32_t x = k_inv;
    q32_t y = 0;
    q32_t z = theta;
    q32_t d,tx,ty,tz;
    printf("   n           x            y            z         -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            0.0);
    for (i=0; i<n; i++) {
        d = ( z>=0 ) ? 0 : -1;
        // d = z >> 31;

        tx = x - ((y>>i)^d)-d;
        ty = y + ((x>>i)^d)-d;
        tz = z - ((sintab[i]^d)-d);
        x = tx;
        y = ty;
        z = tz;
        printf("%5u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(sintab[i])*(z>=0?1.0:-1.0));
    }
    printf("true: %12.8f %12.8f\n",cosf(thetaf),sinf(thetaf));

    return 0;
}
