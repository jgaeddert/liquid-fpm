//
// q32_div.port.c
//
// Portable q32_t fixed-point division implementation
//

#include "liquidfpm.h"

// 
q32_t q32_div(q32_t _x, q32_t _y)
{
    int64_t a = _x;
    int64_t b = _y;

    a <<= q32_fracbits;

    return (q32_t)(a / b);
}
