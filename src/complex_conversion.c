//
// Complex fixed-point / floating-point conversion
//

// convert to floating-point precision
float complex CQ(_fixed_to_float)(CQ(_t) _x)
{
    return Q(_fixed_to_float)(_x.real) + 
           Q(_fixed_to_float)(_x.imag) * _Complex_I;
}

// convert to fixed-point precision
CQ(_t) CQ(_float_to_fixed)(float complex _x)
{
    CQ(_t) y;
    y.real = Q(_float_to_fixed)(crealf(_x));
    y.imag = Q(_float_to_fixed)(cimagf(_x));
    return y;
}


