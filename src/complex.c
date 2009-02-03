//
//
//

void cadd_q32(cq32_t _x, cq32_t _y, cq32_t _sum)
{
    _sum[0] = _x[0] + _y[0];
    _sum[1] = _x[1] + _y[1];
}

void csub_q32(cq32_t _x, cq32_t _y, cq32_t _diff)
{
    _diff[0] = _x[0] - _y[0];
    _diff[1] = _x[1] - _y[1];
}

void cmul_q32(cq32_t _x, cq32_t _y, cq32_t _prod)
{
    _prod[0] = mad_f_mul(_x[0],_y[0]) - mad_f_mul(_x[1],_y[1]);
    _prod[1] = mad_f_mul(_x[0],_y[1]) + mad_f_mul(_x[1],_y[0]);
}

void cdiv_q32(cq32_t _x, cq32_t _y, cq32_t _quot)
{
    //                       (a+jb) (x-jy)
    // (a + jb) / (x + jy) = -------------
    //                       (x+jy) (x-jy)
    //
    //                       (a+jb) (x-jy)
    //                     = -------------
    //                         x^2 + y^2
    
    // TODO: implement this method
}

void conj_q32(cq32_t _x)
{
    _x[1] = -_x[1];
}

void cexp_q32(cq32_t _theta, cq32_t * _y)
{
    // TODO: implement this method

}

void carg_q32(cq32_t _theta, cq32_t * _y)
{
    // TODO: implement this method

}

q32_t cabs_q32(cq32_t _x)
{
    // TODO: implement this method

}

q32_t cabs2_q32(cq32_t _x)
{
    // TODO: implement this method

}

