//
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

int main() {
    unsigned int n = 12;
    double b = 2.0;
    q32_t x = q32_float_to_fixed(sqrt(2.0));

    // generate table
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

    q32_t tn = 0;
    q32_t en = q32_one;
    q32_t un = 0;
    q32_t vn = q32_one;
    int dn;
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n",
            q32_fixed_to_float(tn),
            q32_fixed_to_float(en),
            q32_fixed_to_float(logtab[0]));
    for (i=1; i<n; i++) {
        vn >>= 1;
        while (1) {
            //un = en + q32_mul(en,vn);
            un = en;
            un += en>>i;
            dn = (un <= x);
            printf("%4u %12.8f %12.8f %12.8f %12.8f\n",
                    i,
                    q32_fixed_to_float(un),
                    q32_fixed_to_float(tn),
                    q32_fixed_to_float(en),
                    q32_fixed_to_float(logtab[i]));
            if (dn == 0) break;
            tn += logtab[i];
            en = un;
        }
    }

    return 0;
}
