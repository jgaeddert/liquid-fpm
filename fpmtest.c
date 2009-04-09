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

    // multiply
    printf("testing multiplication...\n");
    q32_t a = q32_float_to_fixed(0.25f);
    q32_t b = q32_float_to_fixed(2.25f);
    q32_t r = q32_mul(a,b);
    printf("0x%.8x * 0x%.8x = 0x%.8x\n", a, b, r);
    printf("%8.6f   * %8.6f   = %8.6f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));

    printf("done.\n");
    return 0;
};


