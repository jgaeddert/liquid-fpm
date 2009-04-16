//
// Test new header
//

#include <stdio.h>
#include <math.h>

#include "include/liquidfpm.h"

void fpmtest_q32_conversion();
void fpmtest_q32_abs();
void fpmtest_q32_add();
void fpmtest_q32_sub();
void fpmtest_q32_mul();
void fpmtest_q32_div();

int main() {
    fpmtest_q32_conversion();
    fpmtest_q32_abs();
    fpmtest_q32_add();
    fpmtest_q32_sub();
    fpmtest_q32_mul();
    fpmtest_q32_div();

    printf("testing complex...\n");
    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);

    printf("testing log2...\n");
    log2_q32( q32_float_to_fixed(0.1f) );

    printf("done.\n");
    return 0;
};


void fpmtest_q32_conversion()
{
    printf("testing conversion...\n");
    float f = 0.9999f;
    q32_t q = q32_float_to_fixed(f);
    printf("    %10.7f > 0x%.8x > %10.7f\n",
        f, q, q32_fixed_to_float(q));
}

void fpmtest_q32_abs()
{
    printf("testing abs...\n");
    float x = -2.45f;
    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_abs(a);
    printf("   |0x%.8x| = 0x%.8x\n", a, b);
    printf("   |%10.7f| = %10.7f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b));
    printf("   computed %10.7f (expected %10.7f)\n",
        q32_fixed_to_float(b), fabsf(x));
}

void fpmtest_q32_add()
{
    printf("testing add...\n");
    float x = 0.25f;
    float y = 2.25f;

    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_float_to_fixed(y);
    q32_t r = q32_add(a,b);
    printf("    0x%.8x + 0x%.8x = 0x%.8x\n", a, b, r);
    printf("    %10.7f + %10.7f = %10.7f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));
    printf("    computed %10.7f (expected %10.7f)\n",
        q32_fixed_to_float(r), x+y);

}

void fpmtest_q32_sub()
{
    printf("testing sub...\n");
    float x = 0.25f;
    float y = 2.25f;

    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_float_to_fixed(y);
    q32_t r = q32_sub(a,b);
    printf("    0x%.8x - 0x%.8x = 0x%.8x\n", a, b, r);
    printf("    %10.7f - %10.7f = %10.7f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));
    printf("    computed %10.7f (expected %10.7f)\n",
        q32_fixed_to_float(r), x-y);

}

void fpmtest_q32_mul()
{
    printf("testing multiplication...\n");
    float x = 1/3.0f;
    float y = 2.25f;

    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_float_to_fixed(y);
    q32_t r = q32_mul(a,b);
    printf("    0x%.8x * 0x%.8x = 0x%.8x\n", a, b, r);
    printf("    %10.7f * %10.7f = %10.7f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));
    printf("    computed %10.7f (expected %10.7f)\n",
        q32_fixed_to_float(r), x*y);
}

void fpmtest_q32_div()
{
    printf("testing div...\n");

}
void fpmtest_q32_dotprod()
{
    printf("testing dot product...\n");
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
}

