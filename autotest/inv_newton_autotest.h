//
// 
//

#ifndef LIQUIDFPM_INV_NEWTON_AUTOTEST_H
#define LIQUIDFPM_INV_NEWTON_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

void inv_newton_autotest()
{
    if (_autotest_verbose)
        printf("testing inv() [Newton]...\n");

    unsigned int n=32;  // precision
    unsigned int num_steps = 32;
    float xmin = 1.0f / Q(_fixed_to_float)(Q(_max));
    float xmax = Q(_fixed_to_float)(Q(_max));
    float dx = (xmax - xmin)/((float)(num_steps-1));
    float tol = 1e-5f;

    // testing variables
    float xf;
    float yf;
    q32_t x;
    q32_t y;
    float ytest;

    unsigned int i;
    xf = xmin;
    for (i=0; i<num_steps; i++) {
        // compute true inverse (floating-point)
        yf = 1.0f / xf;

        // compute fixed-point inverse
        x = Q(_float_to_fixed)(xf);
        y = Q(_inv_newton)(x,n);

        ytest = Q(_fixed_to_float)(y);

        CONTEND_DELTA(yf,ytest,tol);

        if (_autotest_verbose) {
            printf("inv(%12.8f) = %12.8f (%12.8f)\n", xf,
                                                      q32_fixed_to_float(y),
                                                      yf);
        }

        // increment input parameter
        xf += dx;
    }

}

#endif // LIQUIDFPM_INV_NEWTON_AUTOTEST_H
