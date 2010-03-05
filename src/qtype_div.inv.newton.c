/*
 * Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
 * Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
 *                                Institute & State University
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// qtype_div.inv.newton.c
//
// Portable qtype_t fixed-point division implementation using
// Newton's method
//

#include "liquidfpm.internal.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

// 
Q(_t) Q(_div_inv_newton)(Q(_t) _x,
                         Q(_t) _y,
                         unsigned int _n)
{
    // compute inv(y) = 1/y
    Q(_t) y_inv = Q(_inv_newton)(_y,_n);

    // return x*inv(y)
    return Q(_mul)(_x,y_inv);
}
