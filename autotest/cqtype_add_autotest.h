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

#ifndef LIQUIDFPM_CQTYPE_ADD_AUTOTEST_H
#define LIQUIDFPM_CQTYPE_ADD_AUTOTEST_H

#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

// helper function to keep code base small
void CQ(_test_add)(float complex _xf,
                   float complex _yf,
                   float _tol)
{
    // convert to fixed-point
    CQ(_t) x = CQ(_float_to_fixed)(_xf);
    CQ(_t) y = CQ(_float_to_fixed)(_yf);

    // execute operation
    CQ(_t) z = CQ(_add)(x,y);
    float complex zf = _xf + _yf;

    // convert to floating-point
    float complex ztest = CQ(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(crealf(zf),crealf(ztest),_tol);
    CONTEND_DELTA(cimagf(zf),cimagf(ztest),_tol);

    if (_autotest_verbose) {
        printf("(%9.4f,%9.4f) + (%9.4f,%9.4f) = (%9.4f,%9.4f), expected: (%9.4f,%9.4f)\n",
                crealf(_xf), cimagf(_xf),
                crealf(_yf), cimagf(_yf),
                crealf(ztest), cimagf(ztest),
                crealf(zf), cimagf(zf));
    }

}

// helper function to keep code base small
void CQ(_test_add_qtype)(CQ(_t) _x,
                         CQ(_t) _y,
                         float _tol)
{
    // convert to fixed-point
    float complex xf = CQ(_fixed_to_float)(_x);
    float complex yf = CQ(_fixed_to_float)(_y);

    // execute operation(s)
    CQ(_t) z = CQ(_add)(_x,_y);
    float complex zf = xf + yf;

    // convert to floating-point
    float complex ztest = CQ(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(crealf(zf),crealf(ztest),_tol);
    CONTEND_DELTA(cimagf(zf),cimagf(ztest),_tol);

    if (_autotest_verbose) {
        printf("(%9.4f,%9.4f) + (%9.4f,%9.4f) = (%9.4f,%9.4f), expected: (%9.4f,%9.4f)\n",
                crealf(xf), cimagf(xf),
                crealf(yf), cimagf(yf),
                crealf(ztest), cimagf(ztest),
                crealf(zf), cimagf(zf));
    }
}


void cqtype_add_autotest()
{
    float tol = Q(_fixed_to_float)(Q(_min));

    // floatint-point tests
    float complex xf = 1.0f + _Complex_I*0.5f;
    float complex yf = 0.5f - _Complex_I*0.9f;
    CQ(_test_add)(xf,yf,tol);

    // basic tests
    CQ(_t) a;
    CQ(_t) b;

    a.real =  q32_one;      //  1.0
    a.imag = -q32_one;      // -1.0
    b.real =  q32_one>>1;   //  0.5
    b.imag =  q32_one>>1;   //  0.5

    CQ(_test_add_qtype)(a,b,tol);
}

#endif // LIQUIDFPM_CQTYPE_ADD_AUTOTEST_H

