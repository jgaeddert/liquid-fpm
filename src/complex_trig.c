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
// Complex fixed-point trig functions
//

// compute magnitude squared
Q(_t) CQ(_abs2)(CQ(_t) _x)
{
    Q(_t) x2 = Q(_mul)(_x.real,_x.real) +
               Q(_mul)(_x.imag,_x.imag);
    return x2;
}

// compute magnitude
Q(_t) CQ(_abs)(CQ(_t) _x)
{
    unsigned int _n=20; // number of iterations (precision)

    return Q(_sqrt_newton)(CQ(_abs2)(_x), _n);
}

// compute complex sin(x) = (exp(j*x) - exp(-j*x))/(j*2)
CQ(_t) CQ(_csin)(CQ(_t) _x)
{
    unsigned int _n=20; // number of iterations (precision)

    // compute exp(j*_x.real) = cos(_x.real) + j*sin(_x.real)
    // convert angle to qtype format (multiply by scaling factor)
    //      theta = _x.real * 2^(intbits-2) / pi
    // TODO : put this as external constant
    Q(_t) qtype_angle_scalar = Q(_float_to_fixed)(1./Q(_angle_scalar));
    Q(_t) theta = Q(_mul)(_x.real, qtype_angle_scalar);
    Q(_t) qsin;
    Q(_t) qcos;
    Q(_sincos_cordic)(theta, &qsin, &qcos, _n);

    // compute exp(imag(_x)), exp(-imag(_x))
    Q(_t) qexp      = Q(_exp_shiftadd)(_x.imag, _n);
    Q(_t) qexp_inv  = Q(_inv_newton)(qexp, _n);

    // y = sin(_x) = (exp(j*x) - exp(-j*x))/(j*2)
    //         
    // exp(   x) = [cos(x.imag) + j*sin(x.imag)] * exp( x.real)
    // exp( j*x) = [cos(x.real) + j*sin(x.real)] * exp(-x.imag)
    // exp(-j*x) = [cos(x.real) - j*sin(x.real)] * exp( x.imag)
    //
    // y.real =  0.5*sin(x.real)*[exp(-x.imag) - exp(x.imag)]
    // y.imag = -0.5*cos(x.real)*[exp(-x.imag) + exp(x.imag)]
    CQ(_t) y;
    y.real =  Q(_mul)(qsin>>1, qexp_inv + qexp);
    y.imag = -Q(_mul)(qcos>>1, qexp_inv - qexp);

    return y;
}

// compute complex cos(x) = (exp(j*x) + exp(-j*x))/2
CQ(_t) CQ(_ccos)(CQ(_t) _x)
{
    unsigned int _n=20; // number of iterations (precision)

    // compute exp(j*_x.real) = cos(_x.real) + j*sin(_x.real)
    // convert angle to qtype format (multiply by scaling factor)
    //      theta = _x.real * 2^(intbits-2) / pi
    // TODO : put this as external constant
    Q(_t) qtype_angle_scalar = Q(_float_to_fixed)(1./Q(_angle_scalar));
    Q(_t) theta = Q(_mul)(_x.real, qtype_angle_scalar);
    Q(_t) qsin;
    Q(_t) qcos;
    Q(_sincos_cordic)(theta, &qsin, &qcos, _n);

    // compute exp(imag(_x)), exp(-imag(_x))
    Q(_t) qexp      = Q(_exp_shiftadd)(_x.imag, _n);
    Q(_t) qexp_inv  = Q(_inv_newton)(qexp, _n);

    // y = cos(_x) = (exp(j*x) + exp(-j*x))/2
    //
    // y.real = 0.5*cos(x.real)*[exp(-x.imag) + exp(x.imag)]
    // y.imag = 0.5*sin(x.real)*[exp(-x.imag) - exp(x.imag)]
    CQ(_t) y;
    y.real =  Q(_mul)(qcos>>1, qexp_inv + qexp);
    y.imag =  Q(_mul)(qsin>>1, qexp_inv - qexp);

    return y;
}

