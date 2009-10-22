// 
// log2|exp2 shift|add tables
//
// Algorithm from fxtbook, ch. 31
// 

#include "liquidfpm.internal.h"

// Pre-computed look-up table: A[k] = log2( 1 + 2^-k )
const q32_t q32_log2_shiftadd_Ak_tab[32] = {
    0x10000000,    0x095c01a0,    0x05269e10,    0x02b80348,
    0x01663f70,    0x00b5d69c,    0x005b9e5a,    0x002dfca2,
    0x001709c4,    0x000b87c2,    0x0005c499,    0x0002e27b,
    0x00017149,    0x0000b8a7,    0x00005c54,    0x00002e2a,
    0x00001715,    0x00000b8b,    0x000005c5,    0x000002e3,
    0x00000171,    0x000000b9,    0x0000005c,    0x0000002e,
    0x00000017,    0x0000000c,    0x00000006,    0x00000003,
    0x00000001,    0x00000001,    0x00000000,    0x00000000};

// Maximum number of iterations, given the shiftadd_Ak_table
// above.  The shift|add algorithm will hit an infinite loop
// condition for values in the table equal to zero, hence this
// limitation.
const unsigned int q32_log2_shiftadd_nmax = 29;

