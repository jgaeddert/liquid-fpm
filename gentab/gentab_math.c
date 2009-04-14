//
// gentab_math.c
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

#define VERBOSE

// print usage
void usage();

int main(int argc, char * argv[]) {

    // initialize variables, set defaults
    unsigned int fracbits=28;
    unsigned int intbits=4;
    enum {GENTAB_SIN, GENTAB_COS, GENTAB_TAN} type;
    char typename[16];
    strcpy(typename,"sin");
    unsigned int res=256;
    char varname[128];
    FILE * fid = stdout;
    char filename[128];
    strcpy(filename,"stdout");

    // get input options
    int d;
    while ((d = getopt(argc,argv,"")) != EOF) {
        switch (d) {
        case 'f':   fracbits = atoi(optarg);    break;
        case 'i':   intbits = atoi(optarg);     break;
        case 'n':   res = atoi(optarg);         break;
        case 'v':   strcpy(varname,optarg);     break;
        case 'o':
            fid = fopen(optarg,"w");
            if (!fid) {
                fprintf(stderr,"error: gentab_math(), could not open output file %s for writing\n", optarg);
                exit(1);
            }
            break;
        case 't':
            if (strcmp(optarg,"sin")==0) {
                type = GENTAB_SIN;
                strcpy(typename,"sin");
            } else if (strcmp(optarg,"cos")==0) {
                type = GENTAB_COS;
                strcpy(typename,"cos");
            } else if (strcmp(optarg,"tan")==0) {
                type = GENTAB_TAN;
                strcpy(typename,"tan");
            } else {
                fprintf(stderr,"error: gentab_math(), invalid type: %s\n", optarg);
                exit(1);
            }
            break;
        default:
            fprintf(stderr,"error: gentab_math(), invalid option\n");
            exit(1);
        }
    }

    sprintf(varname,"lfpb_tab_%s_%u_%u_%u",typename,fracbits,intbits,res);

    // generate the table

    fprintf(fid,"// %s : auto-generated file (do not edit)\n\n", varname);

    fprintf(fid,"//  fracbits  : %u\n", fracbits);
    fprintf(fid,"//  intbits   : %u\n", intbits);
    fprintf(fid,"//  res       : %u\n", res);
    fprintf(fid,"//  varname   : %s\n", varname);
    fprintf(fid,"//  type      : %s\n", typename);
    fprintf(fid,"//  filename  : %s\n", filename);

    fprintf(fid,"\n");
    fprintf(fid,"#include \"liquidfpm.internal.h\"\n\n");

    fprintf(fid,"const int gentab_var=5;\n");

    fprintf(fid,"\n\n");

    return 0;
}

void usage()
{
    printf("generate table data for liquid fixed-point math library\n");
    printf("gentab_math:\n");
    printf("  -f <fracbits> : number of fractional bits\n");
    printf("  -i <intbits>  : number of integer bits\n");
    printf("  -n <res>      : resolution / number of elements\n");
    printf("  -v <varname>  : table variable name, default: lfpm_tab_<type>_q<fracbits>_<intbits>_<res>\n");
    printf("  -t <type>     : table type [sin,cos,tan,atan,log2]\n");
    printf("  -o <filename> : output filename [stdout default]\n");
}
