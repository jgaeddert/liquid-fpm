//
// Test new header
//

#include <stdio.h>
#include "include/liquidfpm.h"

int main() {
    // conversion
    float f = 0.9999f;
    q32_t q = q32_float_to_fixed(f);
    printf("%8.4f > 0x%.8x > %8.4f\n",
        f, q, q32_fixed_to_float(q));

    q32_t x = 10;
    printf("x = %d\n", x);

    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);


    printf("done.\n");
    return 0;
};


