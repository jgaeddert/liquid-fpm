//
//
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define ACCURACY_LEVEL  0

q32_t log2_q32( q32_t _x )
{
    if (_x <= 0) {
        perror("error: log2_q32(), x must be > 0");
        return 0;
    }
    
    // base index
    int b = msb_index(_x) - 1;

    // compute 8-bit fractional portion
    q32_t f = (b < 8) ? (_x<<(8-b)) : (_x>>(b-8));

    // apply 8-bit mask
    f &= 0x00ff;

    // normalize by decimal position
    b -= q32_fracbits;

#if ACCURACY_LEVEL == 0
    q32_t frac = f << (q32_fracbits-8);
#elif ACCURACY_LEVEL == 1
    q32_t frac = q32_log2_fraction_table[f];
#else
#   error "unknown/unsupported accuracy level"
#endif

    printf("    log2(%f) = %d + %12.10f\n", q32_fixed_to_float(_x), b, q32_fixed_to_float(frac));

    if (b >= 0 ) {
        // resulting log will be positive
        return (b << q32_fracbits) + frac;
    } else {
        // resulting log will be negative
        return -(abs(b) << q32_fracbits) + frac;
    }

    return 0;
}

#if 0
void log2_base_frac_index(q32_t _x, int *_b, int *_f)
{
    // #ifdef FPM_CHECK_INPUT
    //   if (_x <= 0) perror(...);
    // #endif

    *_b = msb_index(_x) - 1;
    if (*_b < 8)
        *_f = _x << (8 - *_b);
    else
        *_f = _x >> (*_b - 8);
}
#endif

