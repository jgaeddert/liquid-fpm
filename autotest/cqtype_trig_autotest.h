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

#ifndef LIQUIDFPM_CQTYPE_TRIG_AUTOTEST_H
#define LIQUIDFPM_CQTYPE_TRIG_AUTOTEST_H

#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

// helper function to keep code base small
void CQ(_test_trig)(float complex _xf,
                    float _tol)
{
    // convert to fixed-point
    CQ(_t) x = CQ(_float_to_fixed)(_xf);

    // execute fixed-point operations
    CQ(_t) cqsin = CQ(_csin)(x);
    CQ(_t) cqcos = CQ(_ccos)(x);
    CQ(_t) cqtan = CQ(_ctan)(x);

    // convert result to floating-point
    float complex cqsinf = CQ(_fixed_to_float)(cqsin);
    float complex cqcosf = CQ(_fixed_to_float)(cqcos);
    float complex cqtanf = CQ(_fixed_to_float)(cqtan);

    // compute expected result (standard math library)
    float complex csinf_test = csinf(_xf);
    float complex ccosf_test = ccosf(_xf);
    float complex ctanf_test = ctanf(_xf);

    if (_autotest_verbose) {
        printf("\n");
        printf("x   = %12.4e + j*%12.4e;\n", crealf(_xf), cimagf(_xf));

        printf("csin(x) = (%9.4f,%9.4f), error: (%12.4e,%12.4e)\n",
                crealf(cqsinf), cimagf(cqsinf),
                crealf(csinf_test-cqsinf), cimagf(csinf_test-cqsinf));

        printf("ccos(x) = (%9.4f,%9.4f), error: (%12.4e,%12.4e)\n",
                crealf(cqcosf), cimagf(cqcosf),
                crealf(ccosf_test-cqcosf), cimagf(ccosf_test-cqcosf));

        printf("ctan(x) = (%9.4f,%9.4f), error: (%12.4e,%12.4e)\n",
                crealf(cqtanf), cimagf(cqtanf),
                crealf(ctanf_test-cqtanf), cimagf(ctanf_test-cqtanf));
    }

    // run comparison
    CONTEND_DELTA(crealf(cqsinf),crealf(csinf_test),_tol);
    CONTEND_DELTA(cimagf(cqsinf),cimagf(csinf_test),_tol);

    CONTEND_DELTA(crealf(cqcosf),crealf(ccosf_test),_tol);
    CONTEND_DELTA(cimagf(cqcosf),cimagf(ccosf_test),_tol);

    CONTEND_DELTA(crealf(cqtanf),crealf(ctanf_test),_tol);
    CONTEND_DELTA(cimagf(cqtanf),cimagf(ctanf_test),_tol);

}


void cqtype_trig_autotest()
{
    // determine appropriate tolerance
#if 0
    unsigned int shift = Q(_intbits) < 6 ? 6 : Q(_intbits);
    float tol = Q(_fixed_to_float)(1<<shift);
    if (tol > 0.1) tol = 0.1f;
#else
    float tol = 1e-3f;
#endif

    if (_autotest_verbose)
        printf("tol : %12.4e;\n", tol);

    // floatint-point tests
    CQ(_test_trig)(0.5f + _Complex_I*1.0f, tol);
    CQ(_test_trig)(0.5f - _Complex_I*1.0f, tol);

    CQ(_test_trig)(1.0f + _Complex_I*0.5f, tol);
    CQ(_test_trig)(1.0f - _Complex_I*0.5f, tol);
}

#endif // LIQUIDFPM_CQTYPE_TRIG_AUTOTEST_H

