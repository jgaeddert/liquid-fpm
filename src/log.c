//
//
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define ACCURACY_LEVEL  1

q32_t log2_q32( q32_t _x )
{
    if (_x <= 0) {
        perror("error: log2_q32(), x must be > 0");
        return 0;
    }

#if ACCURACY_LEVEL == 0
    // TODO: fix this method (does not work for _x > 1.0)

    // base: find most significant bit
    int32_t b = msb_index(_x) - 1;

    printf("0x%.8x > 0x%.8x\n", _x, _x^(1<<b));

    // fractional portion: clear most significant bit, shift result
    q32_t frac = (_x ^ (1<<b)) << (q32_fracbits - b);
    
    // scale base by data type fracbits
    b -= q32_fracbits;
    printf("    log2(%f) = %d + %12.10f\n", q32_fixed_to_float(_x), b, q32_fixed_to_float(frac));

    // add base and fraction, negating base if necessary
    return frac + ((b<0) ? -(abs(b)<<q32_fracbits) : (b<<q32_fracbits));
#elif ACCURACY_LEVEL == 1
    
    // base index
    int b = msb_index(_x) - 1;

    // compute 8-bit fractional portion
    q32_t f = (b < 8) ? (_x<<(8-b)) : (_x>>(b-8));

    // apply 8-bit mask
    f &= 0x00ff;

    // normalize by decimal position
    b -= q32_fracbits;

    q32_t frac = q32_log2_fraction_table[f];
    printf("    log2(%f) = %d + %12.10f\n", q32_fixed_to_float(_x), b, q32_fixed_to_float(frac));

    if (b >= 0 ) {
        // resulting log will be positive
        return (b << q32_fracbits) + frac;
    } else {
        // resulting log will be negative
        return -(abs(b) << q32_fracbits) + frac;
    }

    return 0;
#else
#   error "unknown/unsupported accuracy level"
#endif

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

