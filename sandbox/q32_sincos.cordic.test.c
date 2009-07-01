//
// sin/cos using CORDIC (fixed-point)
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 16;
    double theta = q32_float_to_fixed(M_PI / 3.0);

    // generate table
    q32_t sintab[n];
    double inv_2_n   = 1.0;
    unsigned int i;
    for (i=0; i<n; i++) {
        sintab[i] = q32_float_to_fixed(atan(inv_2_n));
        inv_2_n *= 0.5;
    }

    q32_t k_inv = q32_float_to_fixed(0.6072529350088812561694467525049282631123908521500897724);

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
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n",
            i,
            q32_fixed_to_float(x),
            q32_fixed_to_float(y),
            q32_fixed_to_float(z),
            q32_fixed_to_float(sintab[i])*(z>=0?1.0:-1.0));
    }

    return 0;
}
