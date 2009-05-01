//
// Complex fixed-point arithmetic
//

#include "liquidfpm.internal.h"

#define CQ(name)    LIQUIDFPM_CONCAT(cq32,name)
#define Q(name)     LIQUIDFPM_CONCAT(q32, name)

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

// divide 2 complex numbers
CQ(_t) CQ(_div)(CQ(_t) _a, CQ(_t) _b)
{
    CQ(_t) quot = CQ(_mul)(_a, CQ(_conj)(_b));
    Q(_t) scale = Q(_mul)(_b.real,_b.real) + Q(_mul)(_b.imag,_b.imag);
    quot.real = Q(_div)(quot.real,scale);
    quot.imag = Q(_div)(quot.imag,scale);
    return quot;
}

