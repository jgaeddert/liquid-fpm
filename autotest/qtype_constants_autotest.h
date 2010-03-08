/*
 * Copyright (c) 2008, 2009 Joseph Gaeddert
 * Copyright (c) 2008, 2009 Virginia Polytechnic Institute & State University
 *
 * This file is part of liquid-fpm.
 *
 * liquid-fpm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid-fpm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid-fpm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIQUIDFPM_QTYPE_CONSTANTS_AUTOTEST_H
#define LIQUIDFPM_QTYPE_CONSTANTS_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

void qtype_constants_autotest()
{
    // determine appropriate tolerance
    unsigned int shift = Q(_intbits) < 6 ? 6 : Q(_intbits);
    float tol = Q(_fixed_to_float)(1<<shift);
    if (tol > 0.1f) tol = 0.1f;
    //printf("tol : %16.8e\n", tol);

    CONTEND_DELTA( Q(_fixed_to_float)(Q(_E)),        M_E,       tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_LOG2E)),    M_LOG2E,   tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_LOG10E)),   M_LOG10E,  tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_LN2)),      M_LN2,     tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_LN10)),     M_LN10,    tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_PI)),       M_PI,      tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_PI_2)),     M_PI_2,    tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_PI_4)),     M_PI_4,    tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_1_PI)),     M_1_PI,    tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_2_PI)),     M_2_PI,    tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_2_SQRTPI)), M_2_SQRTPI,tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_SQRT2)),    M_SQRT2,   tol);
    CONTEND_DELTA( Q(_fixed_to_float)(Q(_SQRT1_2)),  M_SQRT1_2, tol);

    if (_autotest_verbose) {
        printf("E        %12.8f (%12.8f) // e\n",          Q(_fixed_to_float)(Q(_E)),          M_E);
        printf("LOG2E    %12.8f (%12.8f) // log2(e)\n",    Q(_fixed_to_float)(Q(_LOG2E)),      M_LOG2E);
        printf("LOG10E   %12.8f (%12.8f) // log10(e)\n",   Q(_fixed_to_float)(Q(_LOG10E)),     M_LOG10E);
        printf("LN2      %12.8f (%12.8f) // log(2)\n",     Q(_fixed_to_float)(Q(_LN2)),        M_LN2);
        printf("LN10     %12.8f (%12.8f) // log(10)\n",    Q(_fixed_to_float)(Q(_LN10)),       M_LN10);
        printf("PI       %12.8f (%12.8f) // pi\n",         Q(_fixed_to_float)(Q(_PI)),         M_PI);
        printf("PI_2     %12.8f (%12.8f) // pi/2\n",       Q(_fixed_to_float)(Q(_PI_2)),       M_PI_2);
        printf("PI_4     %12.8f (%12.8f) // pi/4\n",       Q(_fixed_to_float)(Q(_PI_4)),       M_PI_4);
        printf("1_PI     %12.8f (%12.8f) // 1/pi\n",       Q(_fixed_to_float)(Q(_1_PI)),       M_1_PI);
        printf("2_PI     %12.8f (%12.8f) // 2/pi\n",       Q(_fixed_to_float)(Q(_2_PI)),       M_2_PI);
        printf("2_SQRTPI %12.8f (%12.8f) // 2/sqrt(pi)\n", Q(_fixed_to_float)(Q(_2_SQRTPI)),   M_2_SQRTPI);
        printf("SQRT2    %12.8f (%12.8f) // sqrt(2)\n",    Q(_fixed_to_float)(Q(_SQRT2)),      M_SQRT2);
        printf("SQRT1_2  %12.8f (%12.8f) // 1/sqrt(2)\n",  Q(_fixed_to_float)(Q(_SQRT1_2)),    M_SQRT1_2);
    }

}

#endif // LIQUIDFPM_QTYPE_CONSTANTS_AUTOTEST_H
