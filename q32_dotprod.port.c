//
//
//

//#include <inttypes.h>
//typedef int32_t q32_t;

#include "include/liquidfpm.h"

q32_t q32_dotprod(q32_t * _v, q32_t *_x, unsigned int _n)
{
    unsigned int i;
    q32_t dp = 0;
    for (i=0; i<_n; i++)
        dp += q32_mul(_v[i],_x[i]);

    return dp;
}
