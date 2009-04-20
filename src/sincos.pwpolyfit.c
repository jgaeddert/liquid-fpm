// 
// sin/cos piecewise poly
// 

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG 0

// polynomial coefficients
q32_t q32_sine_table_pwpolyfit[16][3] = {
    {0xff5272e0, 0x19227f80, 0x00000000},
    {0xfd5c32b5, 0x19588040, 0xfffe9636},
    {0xfb6caa21, 0x19ccb320, 0xfff7cdfd},
    {0xf988c869, 0x1a7b9a40, 0xffe80598},
    {0xf7b55f21, 0x1b5fe4c0, 0xffcc2988},
    {0xf5f71601, 0x1c7285e0, 0xffa1ec58},
    {0xf4525ea1, 0x1daad280, 0xff67f963},
    {0xf2cb69c1, 0x1efea6e0, 0xff1e2150},
    {0xf1661c01, 0x20629280, 0xfec57f01},
    {0xf0260441, 0x21ca0900, 0xfe6093dd},
    {0xef0e5281, 0x23279a00, 0xfdf35aa1},
    {0xee21cfe1, 0x246d2c40, 0xfd834fe1},
    {0xed62d701, 0x258c3c40, 0xfd176fc9},
    {0xecd34f01, 0x26761e80, 0xfcb828b9},
    {0xec74a541, 0x271c4300, 0xfc6f42ad},
    {0xec47cb41, 0x27707940, 0xfc47bb9d}
};

q32_t q32_polyval_p2(q32_t _x,
                     q32_t _c0,
                     q32_t _c1,
                     q32_t _c2)
{
    return _c0 + q32_mul(_c1,_x) + q32_mul(_c2,q32_mul(_x,_x));
}

void q32_angle_quadrant_index(q32_t _theta, unsigned int *_quadrant, unsigned int *_index, q32_t * _x)
{
    // Ensure theta >= 0
    // Because the Q4.28 representation of angles is [-2*pi,2*pi] we can guarantee
    // adding 2*pi will make theta non-negative
    if ( _theta < 0 )
        _theta += FPM_Q32_2_PI;

    // _theta now has the form : 0QQB BBBB BBBx xxxx xxxx xxxx xxxx xxxx
    // Q : 2-bit quadrant
    // B : 8-bit phase resolution
    // x : remaining 21 bits phase resolution, ignored

    // Extract quadrant
    //   1. Shift by 29, len(x) + len(B)
    //   2. Mask with 0x0003 to ensure 2-bit value
    *_quadrant = (_theta >> 29) & 0x0003;

    // Extract appropriate phase index:
    //   1. Shift by 25, number of ignored bits in phase
    //   2. Mask with 0x000F (15), the maximum index for lookup table
    *_index = (_theta >> 25) & 0x000F;

    *_x = (_theta >> 1) & 0x0fffffff;
}

q32_t q32_sin( q32_t _theta )
{
    unsigned int quadrant;
    unsigned int index;
    q32_t x;

    // Extract 2-bit quadrant and 8-bit phase index
    // Q1 | Q0
    // ---+---
    // Q2 | Q3
    q32_angle_quadrant_index(_theta, &quadrant, &index, &x);

#if DEBUG
    printf("    theta : %12.8f (0x%.8x > %12.8f)\n", q32_angle_fixed_to_float(_theta), _theta, q32_fixed_to_float(_theta));
    printf("    quadrant : %u\n", quadrant);
    printf("    index    : %u\n", index);
    printf("    x        : %f\n", q32_fixed_to_float(x));
#endif

    // sine: for quadrants Q1 & Q3 phase is inverted
    if (quadrant & 0x01) {
        x = (1<<q32_fracbits) - x;
        index = 15 - index;
#if DEBUG
        printf("    x        : %f (phase inversion)\n", q32_fixed_to_float(x));
        printf("    index    : %u (phase inversion)\n", index);
#endif
    }

    q32_t c2 = q32_sine_table_pwpolyfit[index][0];
    q32_t c1 = q32_sine_table_pwpolyfit[index][1];
    q32_t c0 = q32_sine_table_pwpolyfit[index][2];
#if DEBUG
    printf("    c0 : %12.8f\n", q32_fixed_to_float(c0));
    printf("    c1 : %12.8f\n", q32_fixed_to_float(c1));
    printf("    c2 : %12.8f\n", q32_fixed_to_float(c2));
#endif

    // sine: for quadrants Q2 & Q3 value is negated
    q32_t retval = q32_polyval_p2(x,c0,c1,c2);
    if (quadrant & 0x02)
        retval = -retval;

    return retval;
}

#if 0
q32_t q32_cos( q32_t _theta )
{
    unsigned int quadrant;
    unsigned int index;

    // Extract 2-bit quadrant and 8-bit phase index
    // Q1 | Q0
    // ---+---
    // Q2 | Q3
    angle_quadrant_index_q32(_theta, &quadrant, &index);

    // cosine is sine leading pi/2 (one quadrant)
    quadrant++;

    // cosine: for quadrants Q1 & Q3 phase is inverted
    if (quadrant & 0x01)
        index = 255 - index;

    // cosine: for quadrants Q2 & Q3 value is negated
    if (quadrant & 0x02)
        return -sin_table_q32_256[index];
    else
        return sin_table_q32_256[index];

}

void q32_sincos( q32_t _theta, q32_t *_sin, q32_t *_cos )
{
    unsigned int quadrant;
    unsigned int index;

    // Extract 2-bit quadrant and 8-bit phase index
    // Q1 | Q0
    // ---+---
    // Q2 | Q3
    angle_quadrant_index_q32(_theta, &quadrant, &index);

    if (quadrant == 0) {
        *_sin =  sin_table_q32_256[index];
        *_cos =  sin_table_q32_256[255-index];
    } else if (quadrant == 1) {
        *_sin =  sin_table_q32_256[255-index];
        *_cos = -sin_table_q32_256[index];
    } else if (quadrant == 2) {
        *_sin = -sin_table_q32_256[index];
        *_cos = -sin_table_q32_256[255-index];
    } else {
        *_sin = -sin_table_q32_256[255-index];
        *_cos =  sin_table_q32_256[index];
    }

}
#endif
