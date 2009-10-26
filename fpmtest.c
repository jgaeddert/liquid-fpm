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
void fpmtest_q32_div_inv_newton();
void fpmtest_q32_log2();
void fpmtest_q32_exp2();
void fpmtest_q32_inv_newton();
void fpmtest_q32_sqrt_newton();
void fpmtest_angle();
void fpmtest_sin();
void fpmtest_cos();
void fpmtest_sincos_cordic();
void fpmtest_log2_shiftadd();
void fpmtest_exp2_shiftadd();
void fpmtest_exp_shiftadd();
void fpmtest_exp10_shiftadd();
void fpmtest_atan2();
void fpmtest_atan2_cordic();
void fpmtest_lngamma();
void fpmtest_sinc();

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
    fpmtest_q32_div_inv_newton();
    fpmtest_q32_log2();
    fpmtest_q32_exp2();
    fpmtest_q32_sqrt_newton();
    fpmtest_angle();
    fpmtest_sin();
    fpmtest_cos();
    fpmtest_atan2();

    fpmtest_q32_dotprod();

    printf("testing complex...\n");
    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);

    printf("testing one...\n");
    printf("%12.8f (expected 1.0)\n", q32_fixed_to_float(q32_one));

    printf("testing max...\n");
    printf("%12.8f : 0x%08x (expected 8.0)\n", q32_fixed_to_float(q32_max), q32_max);
    
    fpmtest_sincos_cordic();

    // test log2 shift|add
    fpmtest_log2_shiftadd();

    // test exp2 shift|add
    fpmtest_exp2_shiftadd();

    fpmtest_q32_inv_newton();
    fpmtest_q32_div_inv_newton();

    fpmtest_atan2_cordic();

    fpmtest_lngamma();

    fpmtest_sinc();
    */

    fpmtest_exp_shiftadd();
    fpmtest_exp10_shiftadd();

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

void fpmtest_q32_div_inv_newton()
{
    printf("testing div [inverse|Newton]...\n");
    float x = 1/3.0f;
    float y = 2.25f;
    unsigned int n=16;

    q32_t a = q32_float_to_fixed(x);
    q32_t b = q32_float_to_fixed(y);
    q32_t r = q32_div_inv_newton(a,b,n);
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

void fpmtest_q32_sqrt_newton()
{
    printf("testing sqrt...\n");
    unsigned int n=16;
    float x = 0.75f;
    q32_t sqrt_test = q32_sqrt_newton( q32_float_to_fixed(x), n );
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

void fpmtest_sincos_cordic()
{
    printf("testing sin|cos [cordic]...\n");
    unsigned int i=0, n=20;
    float thetaf;
    q32_t theta;
    q32_t s,c;
    float sf,cf;
    float error;
    float rmse=0.0f;

    //for (i=0; i<n+1; i++) {
        //thetaf = 4.0f * (float)(i) / ((float)(n)) * M_PI - 2*M_PI;
        thetaf = 3.14159 / 4.0f;
        theta  = q32_angle_float_to_fixed(thetaf);

        q32_sincos_cordic(theta,&s,&c,18);
        sf = sinf(thetaf);
        cf = cosf(thetaf);
        error = fabsf(cf - q32_fixed_to_float(c) +
                      sf - q32_fixed_to_float(s) );
        rmse += error*error;

        printf("%4u : theta=%12.8f, cos:%12.8f(%12.8f), sin:%12.8f(%12.8f), e:%12.8f\n",
                i,
                thetaf,
                q32_fixed_to_float(c),
                cosf(thetaf),
                q32_fixed_to_float(s),
                sinf(thetaf),
                error);
    //}
    
    rmse = sqrt(rmse / (float)n);
    printf("rmse : %e\n", rmse);
}


void fpmtest_log2_shiftadd()
{
    printf("testing log2 [shift|add]...\n");
    unsigned int i=0;
    unsigned int n=21;  // number of tests
    unsigned int k=32;  // number of internal iterations (precision)
    float xf, log2xf;
    q32_t x,  log2x;
    float error;
    float rmse=0.0f;

    // determine valid range of inputs
    float x0 = powf(2.0f,-(1<<(q32_intbits-1)));// minimum input
    float x1 = q32_fixed_to_float(q32_max);     // maximum input
    float sigma = powf(x0/x1,1.0f/(float)n);    // multiplier
    
    xf = x1;
    for (i=0; i<n+1; i++) {
        x  = q32_float_to_fixed(xf);

        log2x  = q32_log2_shiftadd(x,k);
        log2xf = log2f(xf);

        error = log2xf - q32_fixed_to_float(log2x);
        rmse += error*error;

        printf("%4u : x:%12.8f, log2(x):%12.8f(%12.8f), e:%12.8f\n",
                i,
                xf,
                q32_fixed_to_float(log2x),
                log2xf,
                error);

        xf *= sigma;
    }
    rmse = sqrt(rmse / (float)n);
    printf("rmse : %e\n", rmse);
}


void fpmtest_exp2_shiftadd()
{
    printf("testing exp2 [shift|add]...\n");
    unsigned int i=0;
    unsigned int n=30;  // number of tests
    unsigned int k=15;  // number of internal iterations (precision)
    float xf, exp2xf;
    q32_t x,  exp2x;
    float error;
    float rmse=0.0f;

    // determine valid range of inputs
    float x0 = -8.0000f;    // minimum input
    float x1 =  2.9999f;    // maximium input
    float dx = (x1-x0)/(float)(n); 

    xf = x0;
    for (i=0; i<n+1; i++) {
        x  = q32_float_to_fixed(xf);

        exp2x  = q32_exp2_shiftadd(x,k);
        exp2xf = exp2f(xf);

        error = exp2xf - q32_fixed_to_float(exp2x);
        rmse += error*error;

        printf("%4u : x:%12.8f, exp2(x):%12.8f(%12.8f), e:%12.8f\n",
                i,
                xf,
                q32_fixed_to_float(exp2x),
                exp2xf,
                error);

        xf += dx;
    }
    rmse = sqrt(rmse / (float)n);
    printf("rmse : %e\n", rmse);
}


void fpmtest_exp_shiftadd()
{
    printf("testing exp [shift|add]...\n");
    unsigned int i=0;
    unsigned int n=30;  // number of tests
    unsigned int k=15;  // number of internal iterations (precision)
    float xf, expxf;
    q32_t x,  expx;
    float error;
    float rmse=0.0f;

    // determine valid range of inputs
    float x0 = -5.5451f;    // minimum input:   ln(2^-8.0f)
    float x1 =  2.0794f;    // maximium input:  ln(8.0f)
    float dx = (x1-x0)/(float)(n); 

    xf = x0;
    for (i=0; i<n+1; i++) {
        x  = q32_float_to_fixed(xf);

        expx  = q32_exp_shiftadd(x,k);
        expxf = expf(xf);

        error = expxf - q32_fixed_to_float(expx);
        rmse += error*error;

        printf("%4u : x:%12.8f, exp(x):%12.8f(%12.8f), e:%12.8f\n",
                i,
                xf,
                q32_fixed_to_float(expx),
                expxf,
                error);

        xf += dx;
    }
    rmse = sqrt(rmse / (float)n);
    printf("rmse : %e\n", rmse);
}


void fpmtest_exp10_shiftadd()
{
    printf("testing exp10 [shift|add]...\n");
    unsigned int i=0;
    unsigned int n=30;  // number of tests
    unsigned int k=15;  // number of internal iterations (precision)
    float xf, exp10xf;
    q32_t x,  exp10x;
    float error;
    float rmse=0.0f;

    // determine valid range of inputs
    float x0 = -2.4082f;    // minimum input:   log10(2^-8.0f)
    float x1 =  0.9030f;    // maximium input:  log10(8.0f)
    float dx = (x1-x0)/(float)(n); 

    xf = x0;
    for (i=0; i<n+1; i++) {
        x  = q32_float_to_fixed(xf);

        exp10x  = q32_exp10_shiftadd(x,k);
        exp10xf = powf(10.0f,xf);

        error = exp10xf - q32_fixed_to_float(exp10x);
        rmse += error*error;

        printf("%4u : x:%12.8f, exp10(x):%12.8f(%12.8f), e:%12.8f\n",
                i,
                xf,
                q32_fixed_to_float(exp10x),
                exp10xf,
                error);

        xf += dx;
    }
    rmse = sqrt(rmse / (float)n);
    printf("rmse : %e\n", rmse);
}


void fpmtest_q32_inv_newton()
{
    printf("testing inv() [Newton]...\n");
    float xf, invxf;
    q32_t x,  invx;
    unsigned int n=16;

    xf = 0.49999651f;
    invxf = 1.0f / xf;
    x = q32_float_to_fixed(xf);
    invx = q32_inv_newton(x,n);

    printf("inv(%12.8f) = %12.8f (%12.8f)\n", xf,
                                              q32_fixed_to_float(invx),
                                              invxf);
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

void fpmtest_atan2_cordic()
{
    printf("testing atan2|cordic...\n");
    float rf = 1.8f;
    float thetaf = M_PI / 2.0f;
    float xf = rf*cosf(thetaf);
    float yf = rf*sinf(thetaf);
    unsigned int n=16;

    q32_t r;
    q32_t theta;
    q32_t x = q32_float_to_fixed(xf);
    q32_t y = q32_float_to_fixed(yf);
    
    q32_atan2_cordic(y,x,&r,&theta,n);

    printf("atan2(%12.8f,%12.8f) = %12.8f (%12.8f)\n",
            yf,
            xf,
            thetaf,
            q32_angle_fixed_to_float(theta));
}

void fpmtest_lngamma()
{
    printf("testing lngamma...\n");
    float zmin = 0.005f;
    float zmax = 7.5f;
    float alpha = 1.05f;

    float zf = zmin;
    float lngammazf;
    q32_t z;
    q32_t lngammaz;

#if 0
    zf = 2.0f;
    z = q32_float_to_fixed(zf);
    lngammaz = q32_lngamma(z);
    lngammazf = q32_fixed_to_float(lngammaz);
    printf("lngamma(%12.8f) = %12.8f (%12.8f)\n",
            zf,
            lngammazf,
            logf(tgamma(zf)));
    return;
#endif

    unsigned int n=0;
    FILE * fid = fopen("fpmtest_lngamma.m","w");
    fprintf(fid,"%% auto-generated file\n");
    fprintf(fid,"clear all;\n");
    fprintf(fid,"close all;\n");
    while (zf < zmax) {
        printf("z : %12.8f\n", zf);
        fprintf(fid,"z(%4u) = %12.4e;\n", n+1,zf);
        z = q32_float_to_fixed(zf);

        // compute lngamma
        lngammaz = q32_lngamma(z);
        lngammazf = q32_fixed_to_float(lngammaz);

        fprintf(fid,"lngammaz(%4u) = %12.4e;\n", n+1,lngammazf);
        zf *= alpha;
        n++;
    }

    // plot results
    fprintf(fid,"figure;\n");
    fprintf(fid,"semilogx(z,log(gamma(z)),z,lngammaz);\n");
    fprintf(fid,"grid on;\n");
    fclose(fid);
    printf("results written to fpmtest_lngamma.m\n");
}

void fpmtest_sinc()
{
    printf("testing sinc...\n");
    float zmin = -7.5f;
    float zmax =  7.5f;
    float dz = 0.1f;

    float zf = zmin;
    float sinczf;
    q32_t z;
    q32_t sincz;

#if 0
    zf = 0.49999651f;
    z = q32_float_to_fixed(zf);
    sincz = q32_sinc(z);
    sinczf = q32_fixed_to_float(sincz);
    printf("sinc(%12.8f) = %12.8f (%12.8f)\n",
            zf,
            sinczf,
            fabsf(zf) > 1e-6f ? sinf(M_PI*zf)/(M_PI*zf)
                              : 1.0f
            );
    return;
#endif

    unsigned int n=0;
    FILE * fid = fopen("fpmtest_sinc.m","w");
    fprintf(fid,"%% auto-generated file\n");
    fprintf(fid,"clear all;\n");
    fprintf(fid,"close all;\n");
    while (zf < zmax) {
        printf("z : %12.8f\n", zf);
        fprintf(fid,"z(%4u) = %12.4e;\n", n+1,zf);
        z = q32_float_to_fixed(zf);

        // compute sinc
        sincz = q32_sinc(z);
        sinczf = q32_fixed_to_float(sincz);

        fprintf(fid,"sincz(%4u) = %12.4e;\n", n+1,sinczf);
        zf += dz;
        n++;
    }

    // plot results
    fprintf(fid,"figure;\n");
    fprintf(fid,"plot(z,sin(pi*z)./(pi*z),z,sincz);\n");
    fprintf(fid,"grid on;\n");
    fclose(fid);
    printf("results written to fpmtest_sinc.m\n");
}

