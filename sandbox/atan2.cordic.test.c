//
// atan2 using CORDIC
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned int n = 16;
    double theta = M_PI / 3.0f;
    double r = 1.8f;

    double xf = r*cos(theta);
    double yf = r*sin(theta);

    // generate table
    double sintab[n];
    double inv_2_n   = 1.0;
    unsigned int i;
    for (i=0; i<n; i++) {
        sintab[i] = atan(inv_2_n);
        inv_2_n *= 0.5;
    }

    double k_inv = 0.6072529350088812561694467525049282631123908521500897724;

    double x = xf;
    double y = yf;
    double z = 0.0;
    double v = 1.0;
    double d,tx,ty,tz;
    printf("   n           x            y            z         -d*An\n");
    printf("init %12.8f %12.8f %12.8f %12.8f\n", x,y,z,0.0);
    for (i=0; i<n; i++) {
        d = ( y>=0.0 ) ? -1 : 1;
        tx = x - d * v * y;
        ty = y + d * v * x;
        tz = z - d * sintab[i];
        x = tx;
        y = ty;
        z = tz;
        v *= 0.5;
        printf("%4u %12.8f %12.8f %12.8f %12.8f\n", i, x, y, z, d*sintab[i]);
    }

    double r_hat = x*k_inv;
    double theta_hat = z;
    printf("res :%12.8f %12.8f\n", r_hat, theta_hat);
    printf("true:%12.8f %12.8f\n", r, theta);

    return 0;
}

