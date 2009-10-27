//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// region boundaries
float r1 = -0.18f;
float r2 =  0.84f;

// polynomials: regions 1, 2, and 3
float pr1[3] = {-0.00222366247658237,   1.98104537987517237,  -1.50937654453105186};
float pr2[4] = {-0.00790733914732779,  -0.09084663076980067,   1.89386805800983127,  -1.55014876006534097};
float pr3[3] = {-0.0324598018218461,   1.3988322953579679,  -0.7739663642620151};

float kaiser(unsigned int _n, unsigned int _N, float _beta);
float loglogbesseli0(float _z);

int main() {
    // Kaiser window paramters
    unsigned int n = 21;    // window length
    float beta = 7.0f;     // taper parameter

    unsigned int k;
    float wk;
    for (k=0; k<n; k++) {
        wk = kaiser(k,n,beta);
        printf("w(%3u) = %12.8f;\n", k+1, wk);
    }

    return 0;
}

float kaiser(unsigned int _n, unsigned int _N, float _beta)
{
    float t = (float)_n - (float)(_N-1)/2;
    float r = 2.0f*t / (float)(_N);

    /*
    float a = besseli_0(_beta*sqrtf(1-r*r));
    float b = besseli_0(_beta);
    float w = a / b;
    */

    float log2a = powf(2.0f, loglogbesseli0(_beta*sqrtf(1-r*r)));
    float log2b = powf(2.0f, loglogbesseli0(_beta));
    float w = powf(2.0f, log2a - log2b);
    return w;
}

float loglogbesseli0(float _z)
{
    if (_z <= 0) {
        printf("error: _z must be greater than zero\n");
        exit(0);
    }

    float t = log2f(_z);
    float y;
    
    if (t < r1) {
        y = pr1[0]*t*t + pr1[1]*t + pr1[2];
    } else if (t < r2) {
        y = pr2[0]*t*t*t + pr2[1]*t*t + pr2[2]*t + pr2[3];
    } else {
        y = pr3[0]*t*t + pr3[1]*t + pr3[2];
    }
    
    return y;
}

