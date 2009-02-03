//
//
//

#include <stdio.h>
#include <stdlib.h>

#include "fpm_internal.h"

q32_t log2_q32( q32_t _x )
{
    if (_x <= 0) {
        perror("error: log2_q32(), x must be > 0");
        return 0;
    }
    
    int b; // base index
    int f; // 8-bit fraction

    log2_base_frac_index(_x, &b, &f);

    // x000,0100 0000 0000... : 1/4  > -2, b=26
    // x000,1000 0000 0000... : 1/2  > -1, b=27
    // x001,0000 0000 0000... : 1    >  0, b=28
    // x010,0000 0000 0000... : 2    >  1, b=29
    // x100,0000 0000 0000... : 4    >  3, b=30

    b -= FRACBITS;
    if (b >= 0 ) {
        // resulting log will be positive
        //return (b << MAD_F_FRACBITS) + log2_fraction_table_q32_256[f];
    } else {
        // resulting log will be negative
        //return -(abs(b) << MAD_F_FRACBITS) - log2_fraction_table_q32_256[f];
    }
    return 0;
}

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

