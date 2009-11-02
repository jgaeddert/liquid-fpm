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
