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
    printf("testing inv() [Newton]...\n");
    unsigned int num_steps = 32;
    float xmin = 1.0f / 7.99f;
    float xmax = 7.99f;
    float dx = (xmax - xmin)/((float)(num_steps-1));
    float tol = 0.1f;

    float xf;
    float yf;
    q32_t x;
    q32_t y;
    float ytest;
    unsigned int n=32;

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

        printf("inv(%12.8f) = %12.8f (%12.8f)\n", xf,
                                                  q32_fixed_to_float(y),
                                                  yf);
        // increment input parameter
        xf += dx;
    }

}

#endif // LIQUIDFPM_INV_NEWTON_AUTOTEST_H
