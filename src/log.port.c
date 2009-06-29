//
// q32_log
//

#include "liquidfpm.h"

// log(2) = 1/log2(e) = 0.693147180559945
#define q32_log_2   0x0b17217f

// log(x) = log2( x ) * log(2)
//        = log2( x ) / log2(e)
q32_t q32_log( q32_t _x )
{
    return q32_mul( q32_log2(_x), q32_log_2);
}
