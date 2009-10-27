//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/liquidfpm.internal.h"

// region boundaries
q32_t r1;
q32_t r2;

// polynomials: regions 1, 2, and 3
float pr1[2] = { 1.99633460984142f,  -1.48964958686991f};
float pr2[3] = {-0.10796167699674f,   1.886907222877594f,  -1.541787670447069};
float pr3[2] = {1.0653957921855701f,  0.0641482899467989f};

q32_t q32_pr1[2];
q32_t q32_pr2[3];
q32_t q32_pr3[2];

q32_t q32_kaiser(unsigned int _n, unsigned int _N, q32_t _beta);
q32_t q32_loglogbesseli0(q32_t _z);

// compute y = 2^x, but store in fixed|float format
q32f_t q32f_exp2_q32(q32_t _x);

int main() {
    // Kaiser window paramters
    unsigned int n = 21;    // window length
    q32_t beta = q32_float_to_fixed(7.0f); // taper parameter

    unsigned int k;
    // convert polynomials to fixed-point
    for (k=0; k<2; k++) {
        q32_pr1[k] = q32_float_to_fixed(pr1[k]);
        q32_pr2[k] = q32_float_to_fixed(pr2[k]);
        q32_pr3[k] = q32_float_to_fixed(pr3[k]);
    }
    q32_pr2[2] = q32_float_to_fixed(pr2[2]);

    // convert boundaries
    r1 = q32_float_to_fixed(-0.2f);
    r2 = q32_float_to_fixed( 1.2f);

    q32_t wk;
    for (k=0; k<n; k++) {
        wk = q32_kaiser(k,n,beta);
        printf("w(%3u) = %12.8f;\n", k+1, q32_fixed_to_float(wk));
    }

    return 0;
}

q32_t q32_kaiser(unsigned int _n, unsigned int _N, q32_t _beta)
{
    unsigned int n=32;

    float tf = (float)_n - (float)(_N-1)/2;
    float rf = 2.0f*tf / (float)(_N);

    q32_t r = q32_float_to_fixed(rf);
    //printf("  r     : %12.8f\n", q32_fixed_to_float(r));
    //printf("  beta  : %12.8f\n", q32_fixed_to_float(_beta));

    /*
    float a = besseli_0(_beta*sqrtf(1-r*r));
    float b = besseli_0(_beta);
    float w = a / b;
    */

    /*
    float log2a = powf(2.0f, loglogbesseli0(_beta*sqrtf(1-r*r)));
    float log2b = powf(2.0f, loglogbesseli0(_beta));
    float w = powf(2.0f, log2a - log2b);
    */

    q32_t g0 = q32_sqrt_newton(q32_one - q32_mul(r,r), n);
    g0 = q32_float_to_fixed(sqrtf(1.0f-rf*rf));
    //printf("g0     = %12.8f\n", q32_fixed_to_float(g0));
    q32_t t0 = q32_loglogbesseli0( q32_mul(_beta,g0) );
    q32_t t1 = q32_loglogbesseli0( _beta );

    //q32f_t log2a = powf(2.0f, loglogbesseli0(_beta*sqrtf(1-r*r)));
    //q32f_t log2b = powf(2.0f, loglogbesseli0(_beta));
    //float w = powf(2.0f, log2a - log2b);

    q32f_t log2a = q32f_exp2_q32(t0);
    q32f_t log2b = q32f_exp2_q32(t1);
    q32f_t d = q32f_sub(log2a,log2b);
    /*
    printf("    d: %12.8f = 2^(%4d) * %12.8f\n",q32f_fixed_to_float(d),
                                                d.base,
                                                q32_fixed_to_float(d.frac));
    */

    // at this point, d.base <= 0
    /*
    if (d.base < -(q32_intbits-1)) {
        printf("underflow\n");
        return 0;
    } else if (d.base > (q32_intbits-1)) {
        // overflow will occur
        printf("overflow\n");
        return q32_max;
    }
    */

    // convert to q32_t
    q32_t p = d.base > 0 ? q32_one <<   d.base :
                           q32_one >> (-d.base);
    p = q32_mul(p,d.frac);
    //printf("    p: %12.8f\n", q32_fixed_to_float(p));

    q32_t w = q32_exp2_shiftadd(p,_n);

    //w = q32_float_to_fixed(powf(2.0f, q32f_fixed_to_float(d)));

    return w;
}

q32_t q32_loglogbesseli0(q32_t _z)
{
    if (_z <= 0) {
        printf("error: _z must be greater than zero\n");
        printf("       %12.8f\n", q32_fixed_to_float(_z));
        exit(0);
    }

    unsigned int _n = 32;

    q32_t t = q32_log2_shiftadd(_z,_n);
    q32_t y;
    
    if (t < r1) {
        //y = pr1[0]*t + pr1[1];
        y = q32_mul(q32_pr1[0],t) + q32_pr1[1];
    } else if (t < r2) {
        //y = pr2[0]*t*t + pr2[1]*t + pr2[2];
        y = q32_mul(q32_mul(q32_pr2[0],t),t) +
            q32_mul(q32_pr2[1],t) +
            q32_pr2[2];
    } else {
        //y = pr3[0]*t + pr3[1];
        y = q32_mul(q32_pr3[0],t) + q32_pr3[1];
    }
    
    return y;
}

q32f_t q32f_exp2_q32(q32_t _x)
{
    unsigned int _n=32;

    q32f_t y;
    y.base = q32_intpart(_x);
    y.frac = q32_exp2_shiftadd(q32_fracpart(_x),_n);
    
    q32f_constrain(&y);

    //printf("exp2(%12.8f) = %12.8f\n", q32_fixed_to_float(_x),
    //                                  q32f_fixed_to_float(y));
    return y;
}

