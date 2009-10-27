//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// region boundaries
float r1 = -0.2f;
float r2 =  1.2f;

// polynomials: regions 1, 2, and 3
float pr1[2] = { 1.99633460984142f,  -1.48964958686991f};
float pr2[3] = {-0.10796167699674f,   1.886907222877594f,  -1.541787670447069};
float pr3[2] = {1.0653957921855701f,  0.0641482899467989f};

float kaiser(unsigned int _n, unsigned int _N, float _beta);
float loglogbesseli0(float _z);

int main() {
    // Kaiser window paramters
    unsigned int n = 21;    // window length
    float beta = 10.0f;     // taper parameter

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
        y = pr1[0]*t + pr1[1];
    } else if (t < r2) {
        y = pr2[0]*t*t + pr2[1]*t + pr2[2];
    } else {
        y = pr3[0]*t + pr3[1];
    }
    
    return y;
}

