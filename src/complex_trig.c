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

// compute complex exponent
CQ(_t) CQ(_cexp)(CQ(_t) _x)
{
    // compute phasor
    // TODO: convert to standard phasor form
    Q(_t) qsin = Q(_sin)(_x.imag);
    Q(_t) qcos = Q(_cos)(_x.imag);

    // compute scaling factor
    Q(_t) scale = Q(_exp)(_x.real);

    CQ(_t) cexp = { Q(_mul)(scale,qcos), Q(_mul)(scale,qsin) };
    return cexp;
}

// compute complex logarithm
CQ(_t) CQ(_clog)(CQ(_t) _x)
{
    // log(x) = log(|x|) + j*arg(x)
    CQ(_t) clog = {Q(_log)(CQ(_abs)(_x)), CQ(_carg)(_x)};
    return clog;
}


