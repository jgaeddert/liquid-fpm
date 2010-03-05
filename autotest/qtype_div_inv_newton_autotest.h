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

#ifndef LIQUIDFPM_QTYPE_div_inv_newton_INV_NEWTON_AUTOTEST_H
#define LIQUIDFPM_QTYPE_div_inv_newton_INV_NEWTON_AUTOTEST_H

#include <stdio.h>
#include <math.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// helper function to keep code base small
void Q(_test_div_inv_newton)(float _xf,
                             float _yf,
                             unsigned int _n,
                             float _tol)
{
    // convert to fixed-point
    Q(_t) x = Q(_float_to_fixed)(_xf);
    Q(_t) y = Q(_float_to_fixed)(_yf);

    // execute operation
    Q(_t) z = Q(_div_inv_newton)(x,y,_n);
    float zf = _xf / _yf;

    // convert to floating-point
    float ztest = Q(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(zf,ztest,_tol);

    if (_autotest_verbose) {
        printf("%12.8f / %12.8f = %12.8f (error: %12.4e)\n",
                _xf,     _yf,     ztest,  zf-ztest);
    }
}

// helper function to keep code base small
void Q(_test_div_inv_newton_qtype)(Q(_t) _x,
                                   Q(_t) _y,
                                   unsigned int _n,
                                   float _tol)
{
    // convert to fixed-point
    float xf = Q(_fixed_to_float)(_x);
    float yf = Q(_fixed_to_float)(_y);

    // execute operation
    Q(_t) z = Q(_div_inv_newton)(_x,_y,_n);
    float zf = xf / yf;

    // convert to floating-point
    float ztest = Q(_fixed_to_float)(z);

    // run comparison
    CONTEND_DELTA(zf,ztest,_tol);

    if (_autotest_verbose) {
        printf("%12.8f / %12.8f = %12.8f (error: %12.4e)\n",
                xf,      yf,      ztest,  zf-ztest);
    }
}


void qtype_div_inv_newton_autotest()
{
    unsigned int n=32;  // precision
    float tol = Q(_fixed_to_float)(1<<Q(_intbits));

    // basic tests
    Q(_test_div_inv_newton)( 0.25f, 2.25f, n, tol);
    Q(_test_div_inv_newton)( 0.25f,-2.25f, n, tol);

    // extremes
    Q(_test_div_inv_newton_qtype)( Q(_max), Q(_one)<<1, n, tol);
    Q(_test_div_inv_newton_qtype)( Q(_max), Q(_max),    n, tol);
    Q(_test_div_inv_newton_qtype)( Q(_one), Q(_max),    n, tol);
}

#endif // LIQUIDFPM_QTYPE_div_inv_newton_INV_NEWTON_AUTOTEST_H

