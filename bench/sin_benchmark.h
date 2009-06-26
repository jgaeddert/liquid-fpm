//
//
//

#ifndef __LIQUIDFPM_SIN_BENCHMARK_H__
#define __LIQUIDFPM_SIN_BENCHMARK_H__

#include <sys/resource.h>
#include <math.h>

#include "liquidfpm.internal.h"

#define DEBUG_SIN_BENCH 0

void precision_sin(unsigned int _res, float * _rmse, float * _abse)
{
    float e, rms_error=0.0f, max_error=0.0f;
    float theta_min = -2*M_PI;
    float theta_max =  2*M_PI;
    float dthetaf = (theta_max-theta_min)/(_res-1);
    float thetaf = theta_min;
    float sin_hat;
    float sf;
    unsigned int i;

    q32_t theta, s;
    for (i=0; i<_res; i++) {
        theta = q32_angle_float_to_fixed( thetaf );

        s = q32_sin(theta);
        sin_hat = q32_fixed_to_float(s);
        sf = sinf(thetaf);

        e = fabsf(sf - sin_hat);
        rms_error += e*e;

        if (e > max_error || i == 0)
            max_error = e;

#if DEBUG_SIN_BENCH
        printf("%4u : sin(%12.8f) = %12.8f (%12.8f, e=%12.8f)\n",
                i, thetaf, sf, q32_fixed_to_float(s), e);
//        printf("e(%4u) = %12.4e;\n", i+1, e);
#endif
        thetaf += dthetaf;
    }
    *_rmse = sqrtf(rms_error/_res);
    *_abse = max_error;
}

void benchmark_sin(struct rusage *_start,
                    struct rusage *_finish,
                    unsigned long int *_num_trials)
{
    unsigned long int i;
    q32_t x;

    // start trials
    getrusage(RUSAGE_SELF, _start);
    for (i=0; i<(*_num_trials); i++) {
        x = q32_sin(0x00000234);
        x = q32_sin(0xf6083018);
        x = q32_sin(0xc8591728);
        x = q32_sin(0x02850337);
    }
    getrusage(RUSAGE_SELF, _finish);

    *_num_trials *= 4;
}

#endif // __LIQUIDFPM_SIN_BENCHMARK_H__
