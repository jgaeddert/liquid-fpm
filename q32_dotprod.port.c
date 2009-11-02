//
//
//

#include "include/liquidfpm.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

Q(_t) Q(_dotprod_port)(Q(_t) * _v,
                       Q(_t) *_x,
                       unsigned int _n)
{
    unsigned int i;
    Q(_t) dp = 0;
    for (i=0; i<_n; i++)
        dp += Q(_mul)(_v[i],_x[i]);

    return dp;
}

Q(_t) Q(_dotprod)(Q(_t) * _v,
                  Q(_t) *_x,
                  unsigned int _n)
{
    return Q(_dotprod_port)(_v,_x,_n);
}

