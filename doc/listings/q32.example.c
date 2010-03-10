// file:    listings/q32.example.c
// build:   gcc -Wall q32.example.c -lliquidfpm -o q32.example

#include <stdio.h>
#include <liquid/liquidfpm.h>

int main() {
    // define fixed-point variables
    q32_t a = q32_one << 1; // 2
    q32_t b = q32_one / 3;  // 1/3
    q32_t c = q32_mul(a,b); // 2/3

    printf("%12.8f * %12.8f = %12.8f\n",
            q32_fixed_to_float(a),
            q32_fixed_to_float(b),
            q32_fixed_to_float(c));

    return 0;
}
