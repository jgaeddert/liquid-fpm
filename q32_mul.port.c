//
//
//

#include "liquidfpm.h"

q32_t q32_mul(q32_t _x, q32_t _y)
{
    int64_t prod = _x * _y;
    return (int32_t) (prod >> 28);
}
