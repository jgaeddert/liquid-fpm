//
// piece-wise approximation to BesselI0 using polynomials
//
// demonstration: Kaiser window
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// region boundaries
float r1 = -0.5f;
float r2 =  2.3f;

// polynomials: regions 1, 2, and 3
float pr1[4] = {-7.04705162125710e-04,  -9.42874618538680e-03,   1.95971055243802e+00,  -1.52624072974733e+00};
float pr2[4] = {-0.0133336423648824,  -0.0797229969889246,   1.8935752640799495,  -1.5530587194031322};
float pr3[4] = {0.00634111260763038,  -0.11754582272034697,   1.76929064812301462,  -1.29581201808317714};

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
    float * p = NULL;
    
    if (t < r1) {
        p = pr1;
    } else if (t < r2) {
        p = pr2;
    } else {
        p = pr3;
    }
    y = p[0]*t*t*t + p[1]*t*t + p[2]*t + p[3];
    
    return y;
}

