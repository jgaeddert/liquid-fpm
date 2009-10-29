//
// q32_mul.port.c
//
// Portable q32_t fixed-point multiplication implementation
//

#include "liquidfpm.h"

// This method loses significant precision by pre-shifting
// out the least significant bits of each argument
q32_t q32_mul(q32_t _x, q32_t _y)
{
    unsigned int s0 = q32_fracbits/2;
    unsigned int s1 = q32_fracbits - s0;
    return (_x >> s0) * (_y >> s1);
}
