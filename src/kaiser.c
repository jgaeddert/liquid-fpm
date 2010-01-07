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
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.internal.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)


// computes y = 2^x where
//   x : Q(_t)  (fixed)
//   y : Q(f_t) (fixed|float)
Q(f_t) Q(f_exp2_qtype)(Q(_t) _x);

// Computes kaiser window:
//    w[k,n;beta] = besseli_0( beta*sqrt(1-r*r) ) / besseli_0(beta)
// where
//    r = (2*k-n+1)/n
//
// besseli_0 is the modified Bessel function of the first kind, order
// zero, and is approximated using a piece-wise polynomial fit to
// log(log(besseli_0(log(z))))
Q(_t) Q(_kaiser)(unsigned int _n, unsigned int _N, Q(_t) _beta)
{
    unsigned int n=32;  // precision

    // validate input
    if (_n > _N) {
        printf("error: qtype_kaiser(), n > N\n");
        exit(1);
    }

    // t = n - (N-1)/2
    // r = 2*t/N
    //   = 2*(n/N) - (N-1)/N
    //   = (2*n - N + 1)/N
    Q(_t) r = Q(_ratio)((_n<<1) - _N + 1, _N, n);


    // a = besseli_0(_beta*sqrtf(1-r*r));
    // b = besseli_0(_beta);
    // w = a / b;


    // t0 = log2(log2(a))
    // t1 = log2(log2(b))
    Q(_t) g0 = Q(_sqrt_newton)(Q(_one) - Q(_mul)(r,r), n);
    Q(_t) t0 = Q(_loglogbesseli0)( Q(_mul)(_beta,g0) );
    Q(_t) t1 = Q(_loglogbesseli0)( _beta );

    // log2a = 2 ^ t0
    // log2b = 2 ^ t1

    Q(f_t) log2a = Q(f_exp2_qtype)(t0); // compute log2(a) = 2^t0 (fixed|float)
    Q(f_t) log2b = Q(f_exp2_qtype)(t1); // compute log2(b) = 2^t1 (fixed|float)

    // w = 2 ^ (log2a - log2b)
    Q(f_t) d = Q(f_sub)(log2a,log2b);   // compute log2(a/b) = log2(a) - log2(b)

    // convert from fixed|float to fixed
    //   p = 2^d.base * d.frac
    // NOTE: because the Kaiser window w[n] is in [0,1],
    //       d should be in [-infty,0]
    Q(_t) p = d.base > 0 ? d.frac <<   d.base : // <- this case should never happen
                           d.frac >> (-d.base);

    // compute w = 2^p
    Q(_t) w = Q(_exp2_shiftadd)(p,n);

    return w;
}

// computes y = 2^x where
//   x : Q(_t)  (fixed)
//   y : Q(f_t) (fixed|float)
Q(f_t) Q(f_exp2_qtype)(Q(_t) _x)
{
    unsigned int _n=32;

    Q(f_t) y;
    y.base = Q(_intpart)(_x);
    y.frac = Q(_exp2_shiftadd)(Q(_fracpart)(_x),_n);
    
    Q(f_constrain)(&y);

    //printf("exp2(%12.8f) = %12.8f\n", Q(_fixed_to_float)(_x),
    //                                  Q(f_fixed_to_float)(y));
    return y;
}

