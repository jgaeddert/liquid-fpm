//
//
//

#ifndef __LIQUIDFPM_ATAN_BENCHMARK_H__
#define __LIQUIDFPM_ATAN_BENCHMARK_H__

#include <sys/resource.h>
#include <math.h>

#include "liquidfpm.internal.h"

#define DEBUG 1

void precision_atan2(unsigned int _res, float * _rmse)
{
    float e, mse=0.0f;
    float theta_min = 0;
    float theta_max = 2*M_PI;
    float dthetaf = (theta_max-theta_min)/(_res-1);
    float thetaf = theta_min;
    float xf, yf;
    unsigned int i;

    q32_t theta, x, y;
    for (i=0; i<_res; i++) {
        xf = cosf(thetaf);
        yf = sinf(thetaf);

        x = q32_float_to_fixed(xf);
        y = q32_float_to_fixed(yf);

        theta = q32_atan2(y,x);

        e = fabsf( thetaf - q32_angle_fixed_to_float(theta) );
        if (e > M_PI)
            e -= 2*M_PI;
        else if (e < -M_PI)
            e += 2*M_PI;

        mse += e*e;

#if DEBUG
        printf("%4u : atan2(%12.8f,%12.8f) = %12.8f (%12.8f, e=%12.8f)\n",
                i, yf, xf, thetaf, q32_angle_fixed_to_float(theta), e);
//        printf("e(%4u) = %12.4e;\n", i+1, e);
#endif

        thetaf += dthetaf;
    }
    *_rmse = sqrtf(mse/_res);
}

void benchmark_atan2(struct rusage *_start,
                    struct rusage *_finish,
                    unsigned long int *_num_trials)
{
    unsigned long int i;
    q32_t x;

    // start trials
    getrusage(RUSAGE_SELF, _start);
    for (i=0; i<(*_num_trials); i++) {
        x = q32_atan2(0x00000234,0x381048fc);
        x = q32_atan2(0xf6083018,0xf028cd2e);
        x = q32_atan2(0xc8591728,0x1820f830);
        x = q32_atan2(0x02850337,0x000128f9);
    }
    getrusage(RUSAGE_SELF, _finish);

    *_num_trials *= 4;
}

#endif // __LIQUIDFPM_ATAN_BENCHMARK_H__
