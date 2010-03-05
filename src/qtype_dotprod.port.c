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
// qtype_dotprod.port.c
//
// Portable qtype_t fixed-point dotprod implementation
//

#include "liquidfpm.internal.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

Q(_t) Q(_dotprod_port)(Q(_t) * _v,
                       Q(_t) *_x,
                       unsigned int _n)
{
    unsigned int i;
    Q(_t) dp = 0;
    for (i=0; i<_n; i++)
        dp += Q(_mul)(_v[i],_x[i]);

    return dp;
}

Q(_t) Q(_dotprod)(Q(_t) * _v,
                  Q(_t) *_x,
                  unsigned int _n)
{
    return Q(_dotprod_port)(_v,_x,_n);
}

