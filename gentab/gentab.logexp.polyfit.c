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
// Table/constant generation for log|exp polynomials
//
// qtype_log2_polyfit:
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "liquidfpm.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

int main() {
    FILE * fid = stdout;
    char qtype[] = "q32";

    fprintf(fid,"// auto-generated file : do not edit\n");
    fprintf(fid,"\n");
    fprintf(fid,"#include \"liquidfpm.internal.h\"\n");
    fprintf(fid,"\n");

    // 
    // qtype_log2_polyfit
    //
    float log2_poly_p0 = -1.685874806524239;
    float log2_poly_p1 =  2.028812209786359;
    float log2_poly_p2 = -0.342937403262120;
    fprintf(fid,"// %s_log2_polyfit\n",qtype);
    fprintf(fid,"const %s_t %s_log2_poly_p0 = 0x%.8x;\n", qtype,qtype, Q(_float_to_fixed)(log2_poly_p0));
    fprintf(fid,"const %s_t %s_log2_poly_p1 = 0x%.8x;\n", qtype,qtype, Q(_float_to_fixed)(log2_poly_p1));
    fprintf(fid,"const %s_t %s_log2_poly_p2 = 0x%.8x;\n", qtype,qtype, Q(_float_to_fixed)(log2_poly_p2));
    fprintf(fid,"\n\n");

    return 0;
}
