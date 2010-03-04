/*
 * Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
 * Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
 *                                Institute & State University
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// Table/constant generation for mathematical transcendentals:
//
// qtype_loglogbesseli0:
//      qtype_loglogbesseli0_r1     :   region 1 boundary
//      qtype_loglogbesseli0_r2     :   region 2 boundary
//      qtype_loglogbesseli0_p1[4]  :   polynomial, region 1
//      qtype_loglogbesseli0_p2[4]  :   polynomial, region 2
//      qtype_loglogbesseli0_p3[4]  :   polynomial, region 3
//
// qtype_lngamma:
//      qtype_log2pi_by_2           :   log(2*pi)/2
//      qtype_inv_12                :   1/12
//
// qtype_sinc:
//      qtype_inv_pi                :   1/pi
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("  u/h   :   print this help file\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    //printf("    o   :   output filename [default: standard output]\n");
}

int main(int argc, char*argv[]) {
    // options
    FILE * fid = stdout;
    char qtype[64] = "q32";
    unsigned int intbits = 7;
    unsigned int fracbits = 25;

    // read options
    int dopt;
    while ((dopt = getopt(argc,argv,"uhn:i:f:")) != EOF) {
        switch (dopt) {
        case 'u':
        case 'h':   usage();                    return 0;
        case 'n':   strncpy(qtype,optarg,64);   break;
        case 'i':   intbits = atoi(optarg);     break;
        case 'f':   fracbits = atoi(optarg);    break;
        default:
            fprintf(stderr,"error: %s, unknown option\n", argv[0]);
            usage();
            return 1;
        }
    }

    // function pointer
    unsigned int n = intbits + fracbits;
    if (n != 8 && n != 16 && n != 32) {
        fprintf(stderr,"error: %s, invalid total bits (%u), must be 8,16,32\n", argv[0], n);
        exit(-1);
    }

    fprintf(fid,"// auto-generated file : do not edit\n");
    fprintf(fid,"\n");
    fprintf(fid,"#include \"liquidfpm.internal.h\"\n");
    fprintf(fid,"\n");

    // 
    // qtype_loglogbesseli0
    //
    float loglogbesseli0_r1 = -0.5f;
    float loglogbesseli0_r2 =  2.3f;
    float loglogbesseli0_p1[4] = {-7.04705162125710e-04f, -9.42874618538680e-03f, 1.95971055243802e+00f,-1.52624072974733e+00};
    float loglogbesseli0_p2[4] = {-0.0133336423648824f,   -0.0797229969889246f,   1.8935752640799495f,  -1.5530587194031322};
    float loglogbesseli0_p3[4] = { 0.00634111260763038f,  -0.11754582272034697f,  1.76929064812301462f, -1.29581201808317714};
    fprintf(fid,"// %s_loglogbesseli0\n",qtype);
    fprintf(fid,"const %s_t %s_loglogbesseli0_r1 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(loglogbesseli0_r1,intbits,fracbits));
    fprintf(fid,"const %s_t %s_loglogbesseli0_r2 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(loglogbesseli0_r2,intbits,fracbits));
    fprintf(fid,"%s_t %s_loglogbesseli0_p1[4] = {0x%.8x, 0x%.8x, 0x%.8x, 0x%.8x};\n",
            qtype,qtype,
            qtype_float_to_fixed(loglogbesseli0_p1[0],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p1[1],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p1[2],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p1[3],intbits,fracbits));
    fprintf(fid,"%s_t %s_loglogbesseli0_p2[4] = {0x%.8x, 0x%.8x, 0x%.8x, 0x%.8x};\n",
            qtype,qtype,
            qtype_float_to_fixed(loglogbesseli0_p2[0],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p2[1],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p2[2],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p2[3],intbits,fracbits));
    fprintf(fid,"%s_t %s_loglogbesseli0_p3[4] = {0x%.8x, 0x%.8x, 0x%.8x, 0x%.8x};\n",
            qtype,qtype,
            qtype_float_to_fixed(loglogbesseli0_p3[0],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p3[1],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p3[2],intbits,fracbits),
            qtype_float_to_fixed(loglogbesseli0_p3[3],intbits,fracbits));
    fprintf(fid,"\n\n");

    // 
    // qtype_lngamma
    //
    float log2pi    = logf(2*M_PI);
    float inv_12    = 1.0f / 12.0f;
    fprintf(fid,"// %s_lngamma\n",qtype);
    fprintf(fid,"const %s_t %s_log2pi = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(log2pi,intbits,fracbits));
    fprintf(fid,"const %s_t %s_inv_12 = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(inv_12,intbits,fracbits));
    fprintf(fid,"\n\n");

    // 
    // qtype_sinc
    //
    float inv_pi    = 1.0f / M_PI;
    fprintf(fid,"// %s_sinc\n",qtype);
    fprintf(fid,"const %s_t %s_inv_pi = 0x%.8x;\n", qtype,qtype, qtype_float_to_fixed(inv_pi,intbits,fracbits));

    return 0;
}
