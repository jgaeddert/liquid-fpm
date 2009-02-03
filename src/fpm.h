// 
// Liquid fixed-point math (FPM) library
//

#include <types.h>

typedef int32_t q32_t;
//typedef int64_t cq32_t;
typedef q32_t[2] cq32_t;

// trig functions
q32_t sin_q32(q32_t _theta);
q32_t cos_q32(q32_t _theta);
void sincos_q32(q32_t _theta, q32_t * _sin, q32_t * _cos);
q32_t atan2_q32(q32_t _r);

// logarithmic functions
q32_t exp_q32(q32_t _x);
q32_t log_q32(q32_t _x);
q32_t log2_q32(q32_t _x);

//
// Complex math
//
void cadd_q32(cq32_t _x, cq32_t _y, cq32_t * _sum);
void csub_q32(cq32_t _x, cq32_t _y, cq32_t * _diff);
void cmul_q32(cq32_t _x, cq32_t _y, cq32_t * _prod);
void cdiv_q32(cq32_t _x, cq32_t _y, cq32_t * _quot);

q32_t creal_q32(cq32_t _x);
q32_t cimag_q32(cq32_t _y);

void cexp_q32(cq32_t _theta, cq32_t * _y);
void carg_q32(cq32_t _theta, cq32_t * _y);

