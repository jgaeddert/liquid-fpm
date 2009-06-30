//
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned int n = 30;
    double b = 2.0;
    double x = sqrt(2.0);

    // generate table
    double logtab[n];
    double inv_log_b = 1.0 / log(b);
    double inv_2_n   = 1.0;
    unsigned int i;
    for (i=0; i<n; i++) {
        logtab[i] = log(1.0 + inv_2_n) * inv_log_b;
        inv_2_n *= 0.5;
    }

    double tn = 0.0;
    double en = 1.0;
    double un = 0.0;
    double vn = 1.0;
    int dn;
    printf("   n           un           tn           en           An\n");
    printf("init            - %12.8f %12.8f %12.8f\n", tn, en, logtab[0]);
    for (i=1; i<n; i++) {
        vn *= 0.5;
        while (1) {
            un = en + en*vn;
            dn = (un <= x);
            printf("%4u %12.8f %12.8f %12.8f %12.8f\n", i, un, tn, en, logtab[i]);
            if (dn == 0) break;
            tn += logtab[i];
            en = un;
        }
    }

    return 0;
}
