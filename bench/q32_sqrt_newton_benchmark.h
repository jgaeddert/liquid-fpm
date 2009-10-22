//
//
//

#ifndef __LIQUIDFPM_SQRT_BENCHMARK_H__
#define __LIQUIDFPM_SQRT_BENCHMARK_H__

#include <sys/resource.h>
#include <math.h>

#include "liquidfpm.h"

#define DEBUG_SQRT_BENCH 0

void precision_q32_sqrt_newton(unsigned int _res, float * _rmse, float *_maxe)
{
    float e, rms_error=0.0f, max_error=0.0f;
    float xf_min = 0.01f;
    float xf_max = 7.5f;
    float dxf = (xf_max-xf_min)/(_res-1);
    float xf = xf_min;
    float yf;
    unsigned int i;
    unsigned int n=6;

    q32_t x, y;
    for (i=0; i<_res; i++) {
        yf = sqrtf(xf);

        x = q32_float_to_fixed(xf);
        y = q32_sqrt_newton(x,n);

        e = fabsf(yf - q32_fixed_to_float(y));
        rms_error += e*e;

        if (e > max_error || i == 0)
            max_error = e;

#if DEBUG_SQRT_BENCH
        printf("%4u : sqrt(%12.8f) = %12.8f (%12.8f, e=%12.8f)\n",
                i, xf, yf, q32_fixed_to_float(y), e);
//        printf("e(%4u) = %12.4e;\n", i+1, e);
#endif

        xf += dxf;
    }
    *_rmse = sqrtf(rms_error/_res);
    *_maxe = max_error;
}

void benchmark_q32_sqrt_newton(struct rusage *_start,
                    struct rusage *_finish,
                    unsigned long int *_num_trials)
{
    unsigned long int i;
    q32_t x;
    unsigned int n=6;

    // start trials
    getrusage(RUSAGE_SELF, _start);
    for (i=0; i<(*_num_trials); i++) {
        x = q32_sqrt_newton(0x00000234,n);
        x = q32_sqrt_newton(0x00083018,n);
        x = q32_sqrt_newton(0x28591728,n);
        x = q32_sqrt_newton(0x02850337,n);
    }
    getrusage(RUSAGE_SELF, _finish);

    *_num_trials *= 4;
}

#endif // __LIQUIDFPM_SQRT_BENCHMARK_H__

