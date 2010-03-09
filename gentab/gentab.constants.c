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
// constant generation
//
// conversion:
//      qtype_angle_scalar      :   (1 << (intbits-2)) / pi
//
// regular constants
//      qtype_E        = 2.71828182845905   : e
//      qtype_LOG2E    = 1.44269504088896   : log2(e)
//      qtype_LOG10E   = 0.434294481903252  : log10(e)
//      qtype_LN2      = 0.693147180559945  : log(2)
//      qtype_LN10     = 2.30258509299405   : log(10)
//      qtype_PI       = 3.14159265358979   : pi (actual)
//      qtype_PI_2     = 1.57079632679490   : pi/2
//      qtype_PI_4     = 0.785398163397448  : pi/4
//      qtype_1_PI     = 0.318309886183791  : 1/pi
//      qtype_2_PI     = 0.636619772367581  : 2/pi
//      qtype_2_SQRTPI = 1.12837916709551   : 2/sqrt(pi)
//      qtype_SQRT2    = 1.41421356237310   : sqrt(2)
//      qtype_SQRT1_2  = 0.707106781186547  : 1/sqrt(2)

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.constants usage:\n");
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

    // validate length
    unsigned int totalbits = intbits + fracbits;
    if (totalbits != 8 && totalbits != 16 && totalbits != 32) {
        fprintf(stderr,"error: %s, invalid total bits (%u), must be 8,16,32\n", argv[0], totalbits);
        exit(-1);
    }

    unsigned int i;

    fprintf(fid,"// auto-generated file : do not edit\n");
    fprintf(fid,"// invoked as : ");
    for (i=0; i<argc; i++)
        fprintf(fid,"%s ", argv[i]);
    fprintf(fid,"\n\n");

    fprintf(fid,"#include \"liquidfpm.internal.h\"\n");
    fprintf(fid,"\n");

    fprintf(fid,"// conversion\n");
    float angle_scalar = (1<<(intbits-2)) / M_PI;
    fprintf(fid,"const %s_t %s_angle_scalar = 0x%.8x; // (1<<(intbits-2))/pi\n",
            qtype, qtype, qtype_float_to_fixed(angle_scalar,intbits,fracbits));
    fprintf(fid,"\n");

    fprintf(fid,"// math constants\n");
    fprintf(fid,"const %s_t %s_E        = 0x%.8x; // e\n",          qtype, qtype, qtype_float_to_fixed(M_E,intbits,fracbits));
    fprintf(fid,"const %s_t %s_LOG2E    = 0x%.8x; // log2(e)\n",    qtype, qtype, qtype_float_to_fixed(M_LOG2E,intbits,fracbits));
    fprintf(fid,"const %s_t %s_LOG10E   = 0x%.8x; // log10(e)\n",   qtype, qtype, qtype_float_to_fixed(M_LOG10E,intbits,fracbits));
    fprintf(fid,"const %s_t %s_LN2      = 0x%.8x; // log(2)\n",     qtype, qtype, qtype_float_to_fixed(M_LN2,intbits,fracbits));
    fprintf(fid,"const %s_t %s_LN10     = 0x%.8x; // log(10)\n",    qtype, qtype, qtype_float_to_fixed(M_LN10,intbits,fracbits));
    fprintf(fid,"const %s_t %s_PI       = 0x%.8x; // pi\n",         qtype, qtype, qtype_float_to_fixed(M_PI,intbits,fracbits));
    fprintf(fid,"const %s_t %s_PI_2     = 0x%.8x; // pi/2\n",       qtype, qtype, qtype_float_to_fixed(M_PI_2,intbits,fracbits));
    fprintf(fid,"const %s_t %s_PI_4     = 0x%.8x; // pi/4\n",       qtype, qtype, qtype_float_to_fixed(M_PI_4,intbits,fracbits));
    fprintf(fid,"const %s_t %s_1_PI     = 0x%.8x; // 1/pi\n",       qtype, qtype, qtype_float_to_fixed(M_1_PI,intbits,fracbits));
    fprintf(fid,"const %s_t %s_2_PI     = 0x%.8x; // 2/pi\n",       qtype, qtype, qtype_float_to_fixed(M_2_PI,intbits,fracbits));
    fprintf(fid,"const %s_t %s_2_SQRTPI = 0x%.8x; // 2/sqrt(pi)\n", qtype, qtype, qtype_float_to_fixed(M_2_SQRTPI,intbits,fracbits));
    fprintf(fid,"const %s_t %s_SQRT2    = 0x%.8x; // sqrt(2)\n",    qtype, qtype, qtype_float_to_fixed(M_SQRT2,intbits,fracbits));
    fprintf(fid,"const %s_t %s_SQRT1_2  = 0x%.8x; // 1/sqrt(2)\n",  qtype, qtype, qtype_float_to_fixed(M_SQRT1_2,intbits,fracbits));

    return 0;
}

