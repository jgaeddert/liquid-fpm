//
// Test new header
//

#include <stdio.h>
#include "include/liquidfpm.h"

int main() {
    q32_t x = 10;
    printf("x = %d\n", x);

    cq32_t y = {3,5};
    printf("y = %d + j %d\n", y.real, y.imag);

    printf("done.\n");
    return 0;
};


