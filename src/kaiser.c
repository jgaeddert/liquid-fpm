//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.internal.h"

q32f_t q32f_exp2_q32(q32_t _x);

q32_t q32_kaiser(unsigned int _n, unsigned int _N, q32_t _beta)
{
    unsigned int n=32;  // precision

    /*
    float tf = (float)_n - (float)(_N-1)/2;
    float rf = 2.0f*tf / (float)(_N);

    q32_t r = q32_float_to_fixed(rf);
    */

    // validate input
    if (_n > _N) {
        printf("error: q32_kaiser(), n > N\n");
        exit(1);
    }

    // t = n - (N-1)/2
    // r = 2*t/N
    //   = 2*(n/N) - (N-1)/N
    //   = (2*n - N + 1)/N
    q32_t r = q32_ratio((_n<<1) - _N + 1, _N, n);

    //printf("    t = %12.8f (%12.8f)\n", tf, q32_fixed_to_float(tq));
    //printf("    r = %12.8f (%12.8f)\n", rf, q32_fixed_to_float(r));

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

    // g0 = sqrt(1 - r*r)
    q32_t g0 = q32_sqrt_newton(q32_one - q32_mul(r,r), n);
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

