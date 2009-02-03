// 
// Trig functions
// 

#include <stdio.h>
#include <stdlib.h>

#include "fpm_internal.h"

void angle_quadrant_index_q32(q32_t _theta, unsigned int *_quadrant, unsigned int *_index)
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
    //   1. Shift by 21, number of ignored bits in phase
    //   2. Mask with 0x00FF (255), the maximum index for lookup table
    *_index = (_theta >> 21) & 0x00FF;
}

q32_t sin_q32( q32_t _theta )
{
    unsigned int quadrant;
    unsigned int index;

    // Extract 2-bit quadrant and 8-bit phase index
    // Q1 | Q0
    // ---+---
    // Q2 | Q3
    angle_quadrant_index_q32(_theta, &quadrant, &index);

    // sine: for quadrants Q1 & Q3 phase is inverted
    if (quadrant & 0x01)
        index = 255 - index;

    // sine: for quadrants Q2 & Q3 value is negated
    if (quadrant & 0x02)
        return -sin_table_q32_256[index];
    else
        return sin_table_q32_256[index];

}

q32_t cos_q32( q32_t _theta )
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

void sincos_q32( q32_t _theta, q32_t *_sin, q32_t *_cos )
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

q32_t atan2_q32( q32_t _y, q32_t _x )
{
    // For all practical purposes we are observing r = y/x so the decimal
    // place in fixed point math can be ignored and we can treat y and x
    // as integers so long as we don't lose resolution

    // negate calculated angle if _y < 0
    int negate = (_y < 0);

    // subtract calculate angle from pi if _x < 0
    int sub_from_pi = (_x < 0);

    // subtract calculated angle from pi/2 if abs(y/x) < 1, or invert
    // y/x to by x/y so that log2 of ratio is positive
    int invert = 0;

    // take absolute value as we are only interested in absolute ratio
    // at this point.
    _y = abs(_y);
    _x = abs(_x);

    // handle zero cases here
    if (_y == 0) {
        // atan( 0 ) = pi when (x<)
        // atan( 0 ) = 0  when (x>0)
        return (sub_from_pi) ? FPM_Q32_PI : 0;
    } else if (_x == 0) {
        // atan( +/- inf) = +/- pi/2
        return (negate) ? -FPM_Q32_PI_BY_2 : FPM_Q32_PI_BY_2;
    }

    // compare x, y; place larger value in y, smaller in x, thus ratio
    // r = y/x is always greater than or equal to 1; keeps log2(y/x)
    // positive
    if (_y < _x) {
        // swap _x and _y values
        q32_t tmp = _x;
        _x = _y;
        _y = tmp;
        invert = 1;
    }

    // Ultimately we want to calculate log2(abs(y/x)).  The following
    // block of code achieves this with simple bit shifts, comparisons
    // and a 32-value lookup table.

    //\todo explain what is going on here to compute log2(abs(y/x))

    // Extract location of MSB for _y and _x; keep values in register as
    // operations on them will be computed very quickly

    register unsigned int by = msb_index( (int) _y ) - 1;
    register unsigned int fy = (by<5) ? _y<<(5-by) : _y>>(by-5);
    fy &= 0x001F;   // (1<<5) - 1

    register unsigned int bx = msb_index( (int) _x ) - 1;
    register unsigned int fx = (bx<5) ? _x<<(5-bx) : _x>>(bx-5);
    fx &= 0x001F;   // (1<<5) - 1

    // use lookup table to improve resolution of fractional part
    fy = log2_fraction_table_32[fy];
    fx = log2_fraction_table_32[fx];

    // log2(_y) ~ bx + fx/32
    // log2(_y) ~ by + fy/32

    // Compute log2 difference. Assuming _y >= _x then
    //   log2(y/x) = by-bx + (fy-fx)/32
    // NOTE: by >= bx because _y >= _x
    if (fy < fx) {
        // fractional part of y is too small; carry 1 from base
        by--;
        fy += 32;  // 1<<5
    }
    by -= bx;
    fy -= fx;

    // Result is now stored in by, fy
    // log(abs(y/x)) ~ by + fy/32

    // Use lookup table to find appropriate angles
    q32_t phi;
    if (by > 7) {
        // index exceeds lookup table size; ratio y/x is very large
        phi = FPM_Q32_PI_BY_2;
    } else {
        // indices within lookup table resolution
        phi = atan_table_q32_32_8[fy][by];
    }

    // invert if necessary
    if (invert) phi = FPM_Q32_PI_BY_2 - phi;

    // if in quadrants 3, 4 subtract from pi
    if (sub_from_pi) phi = FPM_Q32_PI - phi;

    // negate if necessary
    return (negate) ? -phi : phi;
}

