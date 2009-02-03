// 
//
//

#ifndef __LIQUID_FPM_INTERNAL_H__
#define __LIQUID_FPM_INTERNAL_H__

//
// Bit manipulation functions
//

// leading zeros (8-bit)
extern const unsigned char lz8[256];

// count leading zeros
unsigned char clz8(unsigned char _byte) { return lz8[_byte]; }

#endif // __LIQUID_FPM_INTERNAL_H__
