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
// Table/constant generation for atan2 piece-wise polynomials
//
// qtype_atan2_polyfit:
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "liquidfpm.internal.h"

void usage(void)
{
    printf("gentab.logexp.polyfit usage:\n");
    printf("  u/h   :   print this help file\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    //printf("    o   :   output filename [default: standard output]\n");
}

float atan2_pwpolyfit_tab[16][3] = {
  {-0.04771404380169175,   0.00257260067715137,   0.99997904539760862},
  {-2.20702079871975165,   0.33962521215663077,   0.98690840138259572},
  {-3.18176610773422031,   0.56909283644128217,   0.97365438638906532},
  {-1.78025009547461521,   0.01527811562077360,   1.02835957778822529},
  {-0.41649699831392845,  -0.68307892358822708,   1.11776116158593219},
  { 0.38922634110370402,  -1.19353556168153307,   1.19860415502070383},
  { 0.74374119036299269,  -1.46110851560422139,   1.24908705643443918},
  { 0.83052079236018483,  -1.53654467642641879,   1.26547468288957599},
  { 0.78800718843279349,  -1.49295585715799861,   1.25430436100060683},
  { 0.69501218694431410,  -1.38733415684937089,   1.22431374908326918},
  { 0.59194552317321070,  -1.25773945966751621,   1.18357607312715163},
  { 0.49603211355449717,  -1.12534769681107383,   1.13789014900586238},
  { 0.41310653663000840,  -1.00065238343444585,   1.09101405190829737},
  { 0.34387423750160129,  -0.88799265390346627,   1.04518210519920185},
  { 0.28702242401070127,  -0.78844850749900541,   1.00160816878923331},
  { 0.24065007648894654,  -0.70151589420069427,   0.96086581771174773}
};


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

    // 
    // qtype_atan2_pwpoly[16][3]
    //
    fprintf(fid,"// %s_atan2_pwpoly\n",qtype);
    fprintf(fid,"const %s_t %s_atan2_pwpoly_tab[16][3] = {\n", qtype,qtype);
    unsigned int j;
    for (i=0; i<16; i++) {
        fprintf(fid,"    {");
        for (j=0; j<3; j++) {
            float val = atan2_pwpolyfit_tab[i][j];
            fprintf(fid,"0x%.8x%s", qtype_float_to_fixed(val,intbits,fracbits), j<2 ? "," : "");
        }
        fprintf(fid,"}%s\n", i<15 ? "," : "");
    }
    fprintf(fid,"};\n\n");

    return 0;
}
