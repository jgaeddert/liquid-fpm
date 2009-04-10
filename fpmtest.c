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

    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);

    // multiply
    printf("testing multiplication...\n");
    q32_t a = q32_float_to_fixed(0.25f);
    q32_t b = q32_float_to_fixed(2.25f);
    q32_t r = q32_mul(a,b);
    printf("0x%.8x * 0x%.8x = 0x%.8x\n", a, b, r);
    printf("%8.5f   * %8.5f   = %8.5f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));

    // dot product
    float vf[10] = {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
    float xf[10] = {1,0,1,0,1,0,1,0,1,0};
    q32_t v[10], x[10];
    unsigned int i;
    for (i=0; i<10; i++) {
        v[i] = q32_float_to_fixed(vf[i]);
        x[i] = q32_float_to_fixed(xf[i]);
    }

    // compute regular dot product
    float dpf=0.0f;
    for (i=0; i<10; i++)
        dpf += vf[i]*xf[i];

    // compute fixed-point dot product
    q32_t dp = q32_dotprod(x,v,10);

    printf("dp : 0x%.8x (%8.5f, expected %8.5f)\n",
        dp,
        q32_fixed_to_float(dp),
        dpf);

    printf("done.\n");
    return 0;
};


