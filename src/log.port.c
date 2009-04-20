//
// q32_log
//

#include "liquidfpm.h"

// log(x) = log2( x ) * log(2)
q32_t q32_log( q32_t _x )
{
    return q32_mul( q32_log2(_x), 0x0b17217f);
}
