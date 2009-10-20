//
//
//

#include <stdio.h>
#include <string.h>
#include <getopt.h>

void usage(void)
{
    printf("  u/h   :   print this help file\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    printf("    o   :   output filename [default: standard output]\n");
}

void qtype_mul_ppc(FILE * _fid,
                   const char * _name,
                   unsigned int _intbits,
                   unsigned int _fracbits)
{
    fprintf(_fid,";    .section __TEXT,__text,regular,pure_instructions\n");
    fprintf(_fid,";    .section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32\n");
    fprintf(_fid,";    .machine ppc7400\n");
    fprintf(_fid,"    .text\n");
    fprintf(_fid,";    .align 2\n");
    fprintf(_fid,";    .p2align 4,,15\n");
    fprintf(_fid,"    .globl _%s_mul\n", _name);
    fprintf(_fid,"_%s_mul:\n", _name);
    fprintf(_fid,"    ; perform actual computation\n");
    fprintf(_fid,"    mullw   r11,r3,r4   ; multiply lo-order word, store in r11\n");
    fprintf(_fid,"    mulhw   r12,r3,r4   ; multiply hi-order word, store in r12\n");
    fprintf(_fid,"\n");
    fprintf(_fid,"    ;rlmi    ...    ; only available for POWER architectures\n");
    fprintf(_fid,"\n");
    fprintf(_fid,"    srwi    r5,r11,%-2u   ; shift lower >> by %2u\n", _fracbits, _fracbits);
    fprintf(_fid,"    ;andi.   r5,r5,0x0f  ; mask with 0x0f\n");
    fprintf(_fid,"\n");
    fprintf(_fid,"    slwi    r6,r12,%-2u   ; shift upper << by %u\n", _intbits, _intbits);
    fprintf(_fid,"\n");
    fprintf(_fid,"    or      r3,r5,r6    ; mask the upper with the lower\n");
    fprintf(_fid,"\n");
    fprintf(_fid,"    blr                 ; return from routine\n");
    fprintf(_fid,"    .subsections_via_symbols\n");
}

int main(int argc, char *argv[])
{
    // data type name
    char typename[64];
    int  typename_set = 0;

    // number of integer bits
    unsigned int intbits;
    int intbits_set = 0;

    // number of fractional bits
    unsigned int fracbits;
    int fracbits_set = 0;

    // output file
    FILE * fid = stdout;
    int outputfile_set = 0;

    int dopt;
    while ((dopt = getopt(argc,argv,"un:i:f:o:")) != EOF) {
        switch (dopt) {
        case 'u':
        case 'h':
            usage(); return 0;
        case 'n':
            if (strlen(optarg) > 64) {
                printf("error: typename too long\n");
                return -1;
            }
            strcpy(typename,optarg); 
            typename_set = 1;
            break;
        case 'i':
            intbits = atoi(optarg);
            intbits_set = 1;
            break;
        case 'f':
            fracbits = atoi(optarg);
            fracbits_set = 1;
            break;
        case 'o':
            fid = fopen(optarg,"w");
            if (fid == NULL) {
                printf("error: could not open %s for writing\n",optarg);
                return -1;
            }
            outputfile_set = 1;
            break;
        default:
            usage();
            return -1;
        }
    }

    if ( !typename_set || !intbits_set  || !fracbits_set ) {
        printf("error: must specify typename, intbits, and fracbits\n");
        usage();
        return -1;
    } else if ( intbits < 1 ) {
        printf("error: intbits must be at least 1 (sign bit)\n");
        return -1;
    } else if ( (intbits + fracbits) != 32 ) {
        printf("error: intbits (%u) + fracbits (%u) must equal 32\n",intbits,fracbits);
        return -1;
    }

    // run the assembly source code generator
    qtype_mul_ppc(fid,typename,intbits,fracbits);

    // close the file (if not printing to stdout)
    if (outputfile_set)
        fclose(fid);

    return 0;
}

