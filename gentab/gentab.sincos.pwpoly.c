//
// gentab.sincos.pwpoly.c
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/liquidfpm.h"

#define Q(name)     LIQUIDFPM_CONCAT(q32,name)

int main(int argc, char * argv[]) {

    float sine_pwpoly_tab[16][3] = {
        {-0.060522662541018,   1.572056911681783,   0.000000000000000},
        {-0.180985121297741,   1.587105617228409,  -0.000469987617146},
        {-0.299704594399829,   1.616767369563292,  -0.002322714891786},
        {-0.415537748422660,   1.660177846704953,  -0.006389920034732},
        {-0.527369046772096,   1.716057960183830,  -0.013370492257612},
        {-0.634121492911867,   1.782734465474581,  -0.023781856592635},
        {-0.734767002428941,   1.858167007778208,  -0.037915785180656},
        {-0.828336304047983,   1.939981237411416,  -0.055799761507165},
        {-0.913928274244483,   2.025507546203796,  -0.077164923354230},
        {-0.990718615555777,   2.111824899009417,  -0.101421490376865},
        {-1.057967795017763,   2.195809163693769,  -0.127642445077247},
        {-1.115028166269405,   2.274185279619180,  -0.154556083676558},
        {-1.161350206743506,   2.343582549571484,  -0.180547888374104},
        {-1.196487809868082,   2.400592293888733,  -0.203671997319161},
        {-1.220102581317005,   2.441827068940535,  -0.221672366098906},
        {-1.231967097931781,   2.463980625487677,  -0.232013527555895}
    };

    // initialize variables, set defaults
    const char qtype[] = "q32";
    unsigned int tabsize = 16;
    unsigned int polyord = 3;

    // generate header
    unsigned int i;
    unsigned int j;

    printf("// auto-generated file (do not edit)\n\n");

    printf("#include \"liquidfpm.internal.h\"\n\n");

    // generate sine table
    printf("// sine piece-wise polynomial table\n");
    printf("const %s_t %s_sine_pwpoly_tab[%u][%u] = {\n", qtype,qtype,tabsize,polyord);
    for (i=0; i<tabsize; i++) {
        printf("    {");
        for (j=0; j<polyord; j++) {
            printf("0x%.8x", Q(_float_to_fixed)(sine_pwpoly_tab[i][j]));

            if (j != (polyord-1))
                printf(", ");
        }
        printf("}");

        if ( i == (tabsize-1) )
            printf("\n};\n");
        else
            printf(",\n");
    }

    printf("\n\n");

    return 0;
}

