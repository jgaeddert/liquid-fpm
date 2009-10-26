//
// sinh/cosh using CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned int n = 16;
    double theta = 1.0f;

    // generate table
    double sinhtab[n];
    double inv_2_n   = 1.0;
    unsigned int k;
    for (k=1; k<n; k++) {
        inv_2_n *= 0.5;
        sinhtab[k] = atanh(inv_2_n);
    }

    double kp_inv = 1.2074970677630721288777210113109158368;

    double x = kp_inv;
    double y = 0.0;
    double z = theta;
    double v = 1.0;
    double d,tx,ty,tz;
    unsigned int i=4;
    printf("   n           x            y            z         -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n", x,y,z,0.0);
    for (k=1; k<n; k++) {
        v *= 0.5f;
loop1:
        d = ( z>=0.0 ) ? +1.0 : -1.0;
        tx = x + d * v * y;
        ty = y + d * v * x;
        tz = z - d * sinhtab[k];
        x = tx;
        y = ty;
        z = tz;
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n", k, x, y, z, d*sinhtab[k]);
        if (k==i) {
            i = 3*i+1;
            goto loop1;
        }

        // c = x;
        // s = y;
    }

    return 0;
}
