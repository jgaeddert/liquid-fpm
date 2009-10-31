//
// Complex fixed-point arithmetic
//

// compute a number's complex conjugate
CQ(_t) CQ(_conj)(CQ(_t) _x)
{
    CQ(_t) conj = {_x.real, -_x.imag};
    return conj;
}

// add 2 complex numbers
CQ(_t) CQ(_add)(CQ(_t) _a, CQ(_t) _b)
{
    CQ(_t) sum = {_a.real + _b.real, _a.imag + _b.imag};
    return sum;
}

// subtract 2 complex numbers
CQ(_t) CQ(_sub)(CQ(_t) _a, CQ(_t) _b)
{
    CQ(_t) diff = {_a.real - _b.real, _a.imag - _b.imag};
    return diff;
}

// multiply 2 complex numbers
CQ(_t) CQ(_mul)(CQ(_t) _a, CQ(_t) _b)
{
#if 0
    CQ(_t) prod = { Q(_mul)(_a.real,_b.real) - Q(_mul)(_a.imag,_b.imag),
                    Q(_mul)(_a.real,_b.imag) + Q(_mul)(_a.imag,_b.real) };
    return prod;
#else
    // faster method? only requires three multiplications
    Q(_t) k1 = Q(_mul)(_a.real, _b.real + _b.imag);
    Q(_t) k2 = Q(_mul)(_b.imag, _a.real + _a.imag);
    Q(_t) k3 = Q(_mul)(_b.real, _a.imag - _a.real);

    CQ(_t) prod = {k1-k2,k1+k3};
    return prod;
#endif
}

// multiply a complex number by a scalar
CQ(_t) CQ(_mul_scalar)(CQ(_t) _a, Q(_t) _b)
{
    CQ(_t) prod = { Q(_mul)(_a.real,_b),
                    Q(_mul)(_a.imag,_b) };
    return prod;
}

// divide 2 complex numbers
CQ(_t) CQ(_div)(CQ(_t) _a, CQ(_t) _b)
{
    unsigned int _n=32; // number of iterations (precision)

    // compute the raw quotient
    CQ(_t) quot = CQ(_mul)(_a, CQ(_conj)(_b));

    // compute scaling factor (and its inverse)
    Q(_t) scale = Q(_mul)(_b.real,_b.real) + Q(_mul)(_b.imag,_b.imag);
    Q(_t) scale_inv = Q(_inv_newton)(scale,_n);

    // multiply the raw quotient by the inverse of the scaling factor
    quot.real = Q(_mul)(quot.real,scale_inv);
    quot.imag = Q(_mul)(quot.imag,scale_inv);

    return quot;
}

// divide a complex number by a scalar
CQ(_t) CQ(_div_scalar)(CQ(_t) _a, Q(_t) _b)
{
    unsigned int _n=32; // number of iterations (precision)

    // compute the inverse of the scaling factor
    Q(_t) scale_inv = Q(_inv_newton)(_b,_n);

    // compute the raw quotient
    CQ(_t) quot = { Q(_mul)(_a.real,scale_inv),
                    Q(_mul)(_a.imag,scale_inv)  };

    return quot;
}

