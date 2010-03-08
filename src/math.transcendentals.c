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
// math : transcendental functions
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_MATH_TRANSCENDENTALS 0

#define Q(name)         LIQUIDFPM_CONCAT(q32,name)

#if DEBUG_MATH_TRANSCENDENTALS
#   include <math.h>
#endif

// log(gamma(z))
Q(_t) Q(_lngamma)(Q(_t) _z)
{
    unsigned int _n = 10; // number of iterations

    // approximation:
    //   ln(gamma(z)) ~ 0.5*[ln(2*pi) - ln(z)] +
    //                    z*[log(z + 1/(12*z + 0.1/z)) - 1]
    //                ~ 0.5*[ln(2*pi) - ln(z)] + z*[log(z + 1/(12*z)) - 1] for z >> 1
    //
    // NOTE: this approximation is inferior:
    //   ln(gamma(z)) ~ (z-0.5)*ln(z) - z + ln(2*pi)/2 + 1/(12*(z+1))
    //

    // gamma identities:
    //  (1) gamma(1+z) =  z*gamma(z)  ->  lngamma(z)  = lngamma(1+z) - ln(z)
    //  (2) gamma(1-z) = -z*gamma(z)  ->  lngamma(-z) = lngamma(1-z) - ln(-z)
    //if (_z < 0)
    //    return lngamma(1-z) - log(-z);

    // NOTE: because this approximation is not particularly good for z < 1,
    //       it is useful to invoke identity (1) above to improve its
    //       accuracy.  However, this is really only necessary if z < 1.
    if (_z < Q(_one))
        return Q(_lngamma)(_z + Q(_one)) - Q(_log_shiftadd)(_z,_n);

    // variables:
    //   g0 : 0.5*[ln(2*pi) - ln(z)]
    //   g1 : 1/(12*z)
    //   g2 : ln(z + g1)
    Q(_t) g0 = (Q(_log2pi) - Q(_log_shiftadd)(_z,_n))>>1;
#if 0
    Q(_t) g1 = z1 < Q(_one) ? Q(_inv_newton)( (_z<<3) + (_z<<2), _n )   // inv(z*12)
                            : Q(_mul)(Q(_inv_12),Q(_inv_newton)(_z,_n)); // inv(z)*inv(12)
#else
    Q(_t) g1 = Q(_mul)(Q(_inv_12),Q(_inv_newton)(_z,_n)); // inv(z)*inv(12)
#endif
    Q(_t) g2 = Q(_log_shiftadd)(_z + g1, _n);

#if DEBUG_MATH_TRANSCENDENTALS
    float zf    = Q(_fixed_to_float)(_z);
    float g0f   = Q(_fixed_to_float)(g0);
    float g1f   = Q(_fixed_to_float)(g1);
    float g2f   = Q(_fixed_to_float)(g2);
    printf("  z     : %12.8f\n", zf);
    printf("  g0    : %12.8f\n", g0f);
    printf("  g1    : %12.8f\n", g1f);
    printf("  g2    : %12.8f\n", g2f);
    printf("  f(z)  : %12.8f\n", g0f + zf*(g2f-1));
    printf("  true  : %12.8f\n", logf(tgammaf(zf)));
#endif

    return g0 + Q(_mul)(_z,g2-Q(_one));
}

// gamma(z) = exp(lngamma(z))
Q(_t) Q(_gamma)(Q(_t) _z)
{
    unsigned int _n=20;
    return Q(_exp_shiftadd)( Q(_lngamma)(_z), _n );
}

// BesselI_0(z)
Q(_t) Q(_besseli0)(Q(_t) _z)
{
    return 0;
}

// BesselJ_0(z)
Q(_t) Q(_besselj0)(Q(_t) _z)
{
    return 0;
}

// sinc(z) = sin(pi*z)/(pi*z)
Q(_t) Q(_sinc)(Q(_t) _z)
{
    unsigned int _n = 32;

    Q(_t) zmin = Q(_one) >> ( Q(_intbits) - 1 );

    // pi : b0010 0000 0000 0000
    _z = Q(_abs)(_z);

    // compute pi*z, represented in fixed-point angular notation
    Q(_t) pi_z  = _z << (Q(_intbits)-2); // TODO : validate this shift

    // z ~ 0 approximation
    // sinc(z) = gamma(1+z)/gamma(1-z)
    //if (Q(_abs)(_z) < zmin )
    //    return Q(_exp_shiftadd)( -Q(_lngamma)(1+_z) - Q(_lngamma)(1-_z), _n );

    // z ~ 0 approximation
#if 0
    // sinc(z) = \prod_{k=1}^{\infty}{ cos(\pi z / 2^k) }
    if (Q(_abs)(_z) < zmin ) {
        //return Q(_one);

        Q(_t) cos_pi_z_by_2 = Q(_cos_cordic)(pi_z>>1, _n);
        Q(_t) cos_pi_z_by_4 = Q(_cos_cordic)(pi_z>>2, _n);
        Q(_t) cos_pi_z_by_8 = Q(_cos_cordic)(pi_z>>3, _n);
#if DEBUG_MATH_TRANSCENDENTALS
        printf("  pi*z          :   %12.8f\n", Q(_angle_fixed_to_float)(pi_z));
        printf("  cos(pi*z/2)   :   %12.8f\n", Q(_fixed_to_float)(cos_pi_z_by_2));
        printf("  cos(pi*z/4)   :   %12.8f\n", Q(_fixed_to_float)(cos_pi_z_by_4));
        printf("  cos(pi*z/8)   :   %12.8f\n", Q(_fixed_to_float)(cos_pi_z_by_8));
#endif
        return Q(_mul)(cos_pi_z_by_2, Q(_mul)(cos_pi_z_by_4, cos_pi_z_by_8));
    }

#else
    // taylor series expansion:
    //      sinc(z) ~ 1 - (pi*z)^2/6 + (pi*z)^4/120 - (pi*z)^6/5040 + ...
    // NOTE: this expansion works better than cos(pi*z/2)*cos(pi*z/4)*...
    if (Q(_abs)(_z) < zmin ) {
        Q(_t) z2 = Q(_mul)(_z,_z);  // z^2
        Q(_t) z4 = Q(_mul)(z2,z2);  // z^4

        return Q(_one) - Q(_mul)(z2,Q(_pi2_6)) + Q(_mul(z4,Q(_pi4_120)));
    }
#endif

    // sin(pi*z)/(pi*z)

    Q(_t) sin_z = Q(_sin_cordic)( pi_z, _n );

    // invert z
    Q(_t) z_inv = Q(_inv_newton)(_z,_n);

#if DEBUG_MATH_TRANSCENDENTALS
    float zf        = Q(_fixed_to_float)(_z);
    float pizf      = Q(_angle_fixed_to_float)(pi_z);
    float sinzf     = Q(_fixed_to_float)(sin_z);
    float z_invf    = Q(_fixed_to_float)(z_inv);
    printf("  z     :   %12.8f\n", zf);
    printf("  pizf  :   %12.8f\n", pizf);
    printf("  sinz  :   %12.8f\n", sinzf);
    printf("  z_inv :   %12.8f\n", z_invf);
#endif

    return Q(_mul)(sin_z,Q(_mul)(z_inv,Q(_inv_pi)));
}

