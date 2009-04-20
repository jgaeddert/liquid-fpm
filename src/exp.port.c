//
// q32_exp
//

#include "liquidfpm.h"

// exp(x) = 2^( _x/log(2) )
q32_t q32_exp( q32_t _x )
{
    return q32_exp2( q32_mul(_x,0x17154765) );
}
