//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/liquidfpm.internal.h"

#define OUTPUT_FILENAME "q32_kaiser_test.m"

// region boundaries
q32_t r1;
q32_t r2;

// polynomials: regions 1, 2, and 3
float pr1[4] = {-7.04705162125710e-04,  -9.42874618538680e-03,   1.95971055243802e+00,  -1.52624072974733e+00};
float pr2[4] = {-0.0133336423648824,  -0.0797229969889246,   1.8935752640799495,  -1.5530587194031322};
float pr3[4] = {0.00634111260763038,  -0.11754582272034697,   1.76929064812301462,  -1.29581201808317714};

q32_t q32_pr1[4];
q32_t q32_pr2[4];
q32_t q32_pr3[4];

q32_t q32_kaiser(unsigned int _n, unsigned int _N, q32_t _beta);
q32_t q32_loglogbesseli0(q32_t _z);

// compute y = 2^x, but store in fixed|float format
q32f_t q32f_exp2_q32(q32_t _x);

int main() {
    // Kaiser window paramters
    unsigned int n = 21;    // window length
    float betaf = 7.0f;
    q32_t beta = q32_float_to_fixed(betaf); // taper parameter

    //
    FILE * fid = fopen(OUTPUT_FILENAME,"w");
    fprintf(fid,"%% %s : auto-generated file\n", OUTPUT_FILENAME);
    fprintf(fid,"close all;\n");
    fprintf(fid,"clear all;\n");
    fprintf(fid,"n = %u;\n", n);
    fprintf(fid,"beta = %12.8f;\n", betaf);

    unsigned int k;
    // convert polynomials to fixed-point
    for (k=0; k<4; k++) {
        q32_pr1[k] = q32_float_to_fixed(pr1[k]);
        q32_pr2[k] = q32_float_to_fixed(pr2[k]);
        q32_pr3[k] = q32_float_to_fixed(pr3[k]);
    }

    // convert boundaries
    r1 = q32_float_to_fixed(-0.5f);
    r2 = q32_float_to_fixed( 2.3f);

    q32_t wk;
    for (k=0; k<n; k++) {
        wk = q32_kaiser(k,n,beta);
        printf("w(%3u) = %12.8f;\n", k+1, q32_fixed_to_float(wk));
        fprintf(fid,"w0(%3u) = %12.8f;\n", k+1, q32_fixed_to_float(wk));
    }

    fprintf(fid,"\n\n");
    fprintf(fid,"nfft = 512;\n");
    fprintf(fid,"w1 = kaiser(n,beta);\n");
    fprintf(fid,"f = [0:(nfft-1)]/nfft - 0.5;\n");
    fprintf(fid,"W0 = 20*log10(abs(fftshift(fft(w0,nfft))));\n");
    fprintf(fid,"W1 = 20*log10(abs(fftshift(fft(w1,nfft))));\n");
    fprintf(fid,"figure;\n");
    fprintf(fid,"plot(f,W0,'LineWidth',2,f,W1,'LineWidth',1);\n");
    fprintf(fid,"grid on;\n");
    fprintf(fid,"legend('fixed-point','floating-point',1);\n");
    fprintf(fid,"xlabel('Normalized Frequency');\n");
    fprintf(fid,"ylabel('Power Spectral Density [dB]');\n");

    fclose(fid);

    printf("results written to %s\n", OUTPUT_FILENAME);

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

    q32f_t log2a = q32f_exp2_q32(t0);   // compute log2(a) = 2^t0 (fixed|float)
    q32f_t log2b = q32f_exp2_q32(t1);   // compute log2(b) = 2^t1 (fixed|float)
    q32f_t d = q32f_sub(log2a,log2b);   // compute log2(a/b) = log2(a) - log2(b)
    /*
    printf("    d: %12.8f = 2^(%4d) * %12.8f\n",q32f_fixed_to_float(d),
                                                d.base,
                                                q32_fixed_to_float(d.frac));
    */

    // convert from fixed|float to fixed
    //   p = 2^d.base * d.frac
    // NOTE: because the Kaiser window w[n] is in [0,1],
    //       d should be in [-infty,0]
    q32_t p = d.base > 0 ? d.frac <<   d.base : // <- this case should never happen
                           d.frac >> (-d.base);
    //printf("    p: %12.8f\n", q32_fixed_to_float(p));

    // compute w = 2^p
    q32_t w = q32_exp2_shiftadd(p,n);

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
    q32_t * p = NULL;
    
    // set appropriate polynomial based on region
    if (t < r1) {
        p = q32_pr1;
    } else if (t < r2) {
        p = q32_pr2;
    } else {
        p = q32_pr3;
    }
    
    // evaluate polynomial
    y = q32_mul(q32_mul(q32_mul(p[0],t),t),t) +
                q32_mul(q32_mul(p[1],t),t) +
                        q32_mul(p[2],t) +
                                p[3];

    return y;
}

// computes y = 2^x where
//   x : q32_t  (fixed)
//   y : q32f_t (fixed|float)
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

