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
    return (_x >> 14) * (_y >> 14);
}
