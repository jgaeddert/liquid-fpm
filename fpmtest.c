//
// Test new header
//

#include <stdio.h>
#include <math.h>

#include "include/liquidfpm.internal.h"

void fpmtest_q32_conversion();
void fpmtest_q32_intpart();
void fpmtest_q32_fracpart();
void fpmtest_q32_abs();
void fpmtest_q32_add();
void fpmtest_q32_sub();
void fpmtest_q32_mul();
void fpmtest_q32_div();
void fpmtest_q32_log2();
void fpmtest_q32_exp2();
void fpmtest_q32_sqrt();
void fpmtest_angle();
void fpmtest_sin();
void fpmtest_cos();
void fpmtest_atan2();

void fpmtest_q32_dotprod();

int main() {
    /*
    fpmtest_q32_conversion();
    fpmtest_q32_intpart();
    fpmtest_q32_fracpart();
    fpmtest_q32_abs();
    fpmtest_q32_add();
    fpmtest_q32_sub();
    fpmtest_q32_mul();
    fpmtest_q32_div();
    fpmtest_q32_log2();
    fpmtest_q32_exp2();
    fpmtest_q32_sqrt();
    fpmtest_angle();
    fpmtest_sin();
    fpmtest_cos();
    */
    fpmtest_atan2();
    return 0;

    fpmtest_q32_dotprod();

    printf("testing complex...\n");
    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);

    printf("testing log...\n");
    float t = 5.0f;
    q32_t x = q32_log( q32_float_to_fixed(t) );
    printf("log(%f) = %f (%f)\n", t, logf(t), q32_fixed_to_float(x));

    printf("testing exp...\n");
    t = 0.5f;
    x = q32_exp( q32_float_to_fixed(t) );
    printf("exp(%f) = %f (%f)\n", t, expf(t), q32_fixed_to_float(x));

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

void fpmtest_q32_fracpart()
{
    printf("testing fracpart...\n");
    float f = 0.9999f;
    q32_t q = q32_float_to_fixed(f);
    printf("    fracpart(%12.8f) = %d\n",
        f, q32_fracpart(q));
}

void fpmtest_q32_intpart()
{
    printf("testing intpart...\n");
    float f = -1.399f;
    q32_t q = q32_float_to_fixed(f);
    printf("    intpart(%12.8f) = %d\n",
        f, q32_intpart(q));
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
    float x = 1/3.0f;
    float y = 2.25f;

    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_float_to_fixed(y);
    q32_t r = q32_div(a,b);
    printf("    0x%.8x / 0x%.8x = 0x%.8x\n", a, b, r);
    printf("    %10.7f / %10.7f = %10.7f\n",
        q32_fixed_to_float(a),
        q32_fixed_to_float(b),
        q32_fixed_to_float(r));
    printf("    computed %10.7f (expected %10.7f)\n",
        q32_fixed_to_float(r), x/y);

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

    printf("    dotprod : 0x%.8x (%8.5f, expected %8.5f)\n",
        dp,
        q32_fixed_to_float(dp),
        dpf);
}

void fpmtest_q32_log2()
{
    printf("testing log2...\n");
    float x = 0.25f + 0.0001f;
    q32_t log2_test = q32_log2( q32_float_to_fixed(x) );
    printf("    log2: 0x%.8x (%8.5f, expected %8.5f)\n",
        log2_test,
        q32_fixed_to_float(log2_test),
        log2f(x));
}

void fpmtest_q32_exp2()
{
    printf("testing exp2...\n");
    float x = -0.75f;
    q32_t exp2_test = q32_exp2( q32_float_to_fixed(x) );
    printf("    exp2: 0x%.8x (%8.5f, expected %8.5f)\n",
        exp2_test,
        q32_fixed_to_float(exp2_test),
        powf(2.0f,x));
}

void fpmtest_q32_sqrt()
{
    printf("testing sqrt...\n");
    float x = 0.75f;
    q32_t sqrt_test = q32_sqrt( q32_float_to_fixed(x) );
    printf("    sqrt: 0x%.8x (%12.10f, expected %12.10f)\n",
        sqrt_test,
        q32_fixed_to_float(sqrt_test),
        sqrt(x));
}

void fpmtest_angle()
{
    printf("testing sin...\n");
    unsigned int i, n=32;
    float theta0;
    q32_t theta1;
    float theta2;
    for (i=0; i<n+1; i++) {
        theta0 = 4.0f * (float)(i) / ((float)(n)) * M_PI - 2*M_PI;
        theta1 = q32_angle_float_to_fixed(theta0);
        theta2 = q32_angle_fixed_to_float(theta1);
        printf("%4u : %12.8f > 0x%.8x > %12.8f\n", i, theta0, theta1, theta2);
    }
}

void fpmtest_sin()
{
    printf("testing sin...\n");
    unsigned int i, n=45;
    float thetaf;
    q32_t theta;
    q32_t s;
#if 1
    for (i=0; i<n+1; i++) {
        thetaf = 4.0f * (float)(i) / ((float)(n)) * M_PI - 2*M_PI;
        theta  = q32_angle_float_to_fixed(thetaf);

        s = q32_sin(theta);

        printf("%4u : sin(%12.8f) = %12.8f (%12.8f) error: %12.8f\n",
                i, thetaf, sinf(thetaf), q32_fixed_to_float(s),
                sinf(thetaf)-q32_fixed_to_float(s));
    }
#else
    thetaf = 2*M_PI;
    theta  = q32_angle_float_to_fixed(thetaf);
    s = q32_sin(theta);
    printf("sin(%12.8f) = %12.8f (%12.8f)\n",
            thetaf, sinf(thetaf), q32_fixed_to_float(s));
#endif
}

void fpmtest_cos()
{
    printf("testing sin...\n");
    unsigned int i, n=45;
    float thetaf;
    q32_t theta;
    q32_t s;
#if 1
    for (i=0; i<n+1; i++) {
        thetaf = 4.0f * (float)(i) / ((float)(n)) * M_PI - 2*M_PI;
        theta  = q32_angle_float_to_fixed(thetaf);

        s = q32_cos(theta);

        printf("%4u : cos(%12.8f) = %12.8f (%12.8f) error: %12.8f\n",
                i, thetaf, cosf(thetaf), q32_fixed_to_float(s),
                cosf(thetaf)-q32_fixed_to_float(s));
    }
#else
    thetaf = 2*M_PI;
    theta  = q32_angle_float_to_fixed(thetaf);
    s = q32_cos(theta);
    printf("cos(%12.8f) = %12.8f (%12.8f)\n",
            thetaf, cosf(thetaf), q32_fixed_to_float(s));
#endif
}

void fpmtest_atan2()
{
    printf("testing atan2...\n");
    float thetaf = 0.234;
    float xf = cosf(thetaf);
    float yf = sinf(thetaf);

    q32_t theta;
    q32_t x = q32_float_to_fixed(xf);
    q32_t y = q32_float_to_fixed(yf);
    
    theta  = q32_atan2(y,x);

    printf("atan2(%12.8f,%12.8f) = %12.8f (%12.8f)\n",
            yf,xf,thetaf, q32_angle_fixed_to_float(theta));
}
