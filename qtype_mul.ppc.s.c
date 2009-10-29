//
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void usage(void)
{
    printf("  u/h   :   print this help file\n");
    printf("    a   :   target architecture: ppc, x86, intelmac\n");
    printf("    n   :   name (e.g. q32b16)\n");
    printf("    i   :   intbits (including sign bit)\n");
    printf("    f   :   fracbits\n");
    printf("    o   :   output filename [default: standard output]\n");
}

//typedef void (*qtype_mul_func)(FILE*, const char*, unsigned int, unsigned int);
//qtype_mul_func qtype_mul_ppc;

void qtype_mul_ppc(FILE * _fid,
                   const char * _name,
                   unsigned int _intbits,
                   unsigned int _fracbits);

void qtype_mul_x86(FILE * _fid,
                   const char * _name,
                   unsigned int _intbits,
                   unsigned int _fracbits);

void qtype_mul_intelmac(FILE * _fid,
                        const char * _name,
                        unsigned int _intbits,
                        unsigned int _fracbits);

int main(int argc, char *argv[])
{
    // data type name
    char typename[64];
    int  typename_set=0;

    // target architecture (function pointer to
    // source code generator)
    void (*func)(FILE *, const char *, unsigned int, unsigned int) = NULL;
    int arch_set=0;

    // number of integer bits
    unsigned int intbits=0;
    int intbits_set=0;

    // number of fractional bits
    unsigned int fracbits=0;
    int fracbits_set=0;

    // output file
    char outputfile[64];
    int outputfile_set=0;

    int dopt;
    while ((dopt = getopt(argc,argv,"ua:n:i:f:o:")) != EOF) {
        switch (dopt) {
        case 'u':
        case 'h':
            usage(); return 0;
        case 'a':
            if (strcmp(optarg,"ppc")==0) {
                // Building for PowerPC
                func = &qtype_mul_ppc;
            /*
            } else if (strcmp(optarg,"x86")==0) {
                // Building for x86
            } else if (strcmp(optarg,"intelmac")==0) {
                // Building for Intel mac
            */
            } else {
                fprintf(stderr,"error: unknown architecture: %s\n", optarg);
                exit(1);
            }
            arch_set = 1;
            break;
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
            if (strlen(optarg) > 64) {
                printf("error: output filename too long\n");
                return -1;
            }
            strcpy(outputfile,optarg); 
            outputfile_set = 1;
            break;
        default:
            usage();
            return -1;
        }
    }

    if ( !arch_set || !typename_set || !intbits_set  || !fracbits_set ) {
        fprintf(stderr,"error: must specify architecture, typename, intbits, and fracbits\n");
        usage();
        return -1;
    } else if ( intbits < 1 ) {
        fprintf(stderr,"error: intbits must be at least 1 (sign bit)\n");
        return -1;
    } else if ( (intbits + fracbits) != 32 ) {
        fprintf(stderr,"error: intbits (%u) + fracbits (%u) must equal 32\n",intbits,fracbits);
        return -1;
    }

    // open output file
    FILE * fid;
    if (outputfile_set) {
        fid = fopen(outputfile,"w");
        if (fid == NULL) {
            printf("error: could not open %s for writing\n",outputfile);
            return -1;
        }
    } else {
        fid = stdout;
    }

    // run the assembly source code generator
    func(fid,typename,intbits,fracbits);

    // close the file (if not printing to stdout)
    if (outputfile_set)
        fclose(fid);

    return 0;
}

void qtype_mul_ppc(FILE * _fid,
                   const char * _name,
                   unsigned int _intbits,
                   unsigned int _fracbits)
{
    fprintf(_fid,"; auto-generated file\n");
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

