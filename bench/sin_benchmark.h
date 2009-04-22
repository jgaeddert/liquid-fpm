//
//
//

#ifndef __LIQUIDFPM_SIN_BENCHMARK_H__
#define __LIQUIDFPM_SIN_BENCHMARK_H__

#include <sys/resource.h>
#include <math.h>

#include "liquidfpm.internal.h"

#ifdef DEBUG
#   undef DEBUG
#endif
#define DEBUG 0

void precision_sin(unsigned int _res, float * _rmse, float * _abse, FILE * _precision_fid)
{
    float e, mse=0.0f;
    float theta_min = -2*M_PI;
    float theta_max =  2*M_PI;
    float dthetaf = (theta_max-theta_min)/(_res-1);
    float thetaf = theta_min;
    float sin_hat;
    float sf;
    unsigned int i;

    fprintf(_precision_fid,"\n\n%% sin\n");
    fprintf(_precision_fid,"clear all;\n");
    fprintf(_precision_fid,"x = zeros(1,%u);\n", _res);
    fprintf(_precision_fid,"y = zeros(1,%u);\n", _res);
    fprintf(_precision_fid,"y_hat = zeros(1,%u);\n", _res);

    q32_t theta, s;
    for (i=0; i<_res; i++) {
        theta = q32_angle_float_to_fixed( thetaf );

        s = q32_sin(theta);
        sin_hat = q32_fixed_to_float(s);
        sf = sinf(thetaf);

        e = sf - sin_hat;
        mse += e*e;

#if DEBUG
        printf("%4u : sin(%12.8f) = %12.8f (%12.8f, e=%12.8f)\n",
                i, thetaf, sf, q32_fixed_to_float(s), e);
//        printf("e(%4u) = %12.4e;\n", i+1, e);
#endif
        fprintf(_precision_fid, "x(%4u) = %12.4e; ", i+1, thetaf);
        fprintf(_precision_fid, "y(%4u) = %12.4e; ", i+1, sinf(thetaf));
        fprintf(_precision_fid, "y_hat(%4u) = %12.4e; ", i+1, sin_hat);
        fprintf(_precision_fid, "\n");

        thetaf += dthetaf;
    }
    *_rmse = sqrtf(mse/_res);
    *_abse = 0.0f;

    fprintf(_precision_fid, "figure; plot(x,y-y_hat);\n");
    fprintf(_precision_fid, "xlabel('x');\n");
    fprintf(_precision_fid, "ylabel('sin(x)');\n");
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
