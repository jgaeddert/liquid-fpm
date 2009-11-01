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

#ifndef LIQUIDFPM_QTYPE_DIV_AUTOTEST_H
#define LIQUIDFPM_QTYPE_DIV_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

// helper function to keep code base small
void Q(_test_div)(float _xf,
                  float _yf,
                  float _tol)
{
    // convert to fixed-point
    Q(_t) x = Q(_float_to_fixed)(_xf);
    Q(_t) y = Q(_float_to_fixed)(_yf);

    // execute operation
    Q(_t) z = Q(_div)(x,y);
    float zf = _xf / _yf;

    // convert to floating-point
    float ztest = Q(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(zf,ztest,_tol);

    if (_autotest_verbose) {
        printf("%12.8f / %12.8f = %12.8f (%12.8f)\n",
                _xf,     _yf,     ztest,  zf);
    }
}

// helper function to keep code base small
void Q(_test_div_qtype)(Q(_t) _x,
                        Q(_t) _y,
                        float _tol)
{
    // convert to fixed-point
    float xf = Q(_fixed_to_float)(_x);
    float yf = Q(_fixed_to_float)(_y);

    // execute operation
    Q(_t) z = Q(_div)(_x,_y);
    float zf = xf / yf;

    // convert to floating-point
    float ztest = Q(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(zf,ztest,_tol);

    if (_autotest_verbose) {
        printf("%12.8f / %12.8f = %12.8f (%12.8f)\n",
                xf,      yf,      ztest,  zf);
    }
}

void qtype_div_autotest()
{
    float tol = Q(_fixed_to_float)(Q(_min)<<4);

    // basic tests
    Q(_test_div)( 0.25f, 2.25f, tol);
    Q(_test_div)( 0.25f,-2.25f, tol);

    // extremes
    Q(_t) x = Q(_max);      // max
    Q(_t) y = Q(_one)<<1;   // 2
    Q(_test_div_qtype)(x,y,tol);
}

#endif // LIQUIDFPM_QTYPE_DIV_AUTOTEST_H

