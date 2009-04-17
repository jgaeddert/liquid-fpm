// 
// Test Newton's method for computing a square root
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned int n = 123456;
    float nf = (float) n;

    unsigned int i;
    unsigned int x0=n, x1;
    for (i=0; i<20; i++) {
        printf("%4u : %u\n", i, x0);
        x1 = (x0 + n/x0)/2;
        x0 = x1;
    }

    printf("sqrt(%f) = %f (approximated as %u)\n", nf, sqrt(nf), x1);
    printf("done.\n");
    return 0;
}
