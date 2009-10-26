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

// compute complex exponent
//      exp(x) = exp(x.real) * [cos(x.imag) + j*sin(x.imag)]
CQ(_t) CQ(_cexp)(CQ(_t) _x)
{
    unsigned int _n=20; // number of iterations (precision)

    // run CORDIC to compute sin|cos from angle
    Q(_t) qsin;     // pure in-phase component
    Q(_t) qcos;     // pure quadrature component
    Q(_sincos_cordic)(_x.imag, &qsin, &qcos, _n);

    // compute scaling factor
    Q(_t) scale = Q(_exp_shiftadd)(_x.real, _n);

    // complex exponent
    CQ(_t) cexp = { Q(_mul)(scale,qcos), Q(_mul)(scale,qsin) };
    return cexp;
}

// compute complex logarithm:
//      log(x) = log(|x|) + j*arg(x)
CQ(_t) CQ(_clog)(CQ(_t) _x)
{
    unsigned int _n=20; // number of iterations (precision)

    Q(_t) r;        // magnitude
    Q(_t) theta;    // angle

    // run CORDIC to compute both magnitude and angle of
    // complex phasor
    Q(_atan2_cordic)(_x.imag, _x.real, &r, &theta, _n);

    // complex logarithm
    CQ(_t) clog = { Q(_log_shiftadd)(Q(_abs)(r),_n), theta };
    return clog;
}


