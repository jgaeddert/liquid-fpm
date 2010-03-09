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

int main(int argc, char *argv[])
{
    // data type name
    char typename[64];
    int  typename_set=0;

    // target architecture specifics
    char comment_char = '#';    // comment character
    char name_mangler[8];       // name-mangling string (mainly for darwin)
    enum {  ARCH_UNKNOWN=0,
            ARCH_PPC,
            ARCH_X86,
            ARCH_X86_64,
            ARCH_INTELMAC } arch=0;
    char archname[64];
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
                arch = ARCH_PPC;
            } else if (strcmp(optarg,"x86")==0) {
                // Building for x86
                arch = ARCH_X86;
            } else if (strcmp(optarg,"intelmac")==0) {
                // Building for Intel mac
                arch = ARCH_INTELMAC;
            } else {
                fprintf(stderr,"error: unknown architecture: %s\n", optarg);
                exit(1);
            }
            arch_set = 1;
            break;
        case 'n':
            if (strlen(optarg) >= 64) {
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

    unsigned int totalbits = intbits + fracbits;
    if ( !arch_set || !typename_set || !intbits_set  || !fracbits_set ) {
        fprintf(stderr,"error: must specify architecture, typename, intbits, and fracbits\n");
        usage();
        return -1;
    } else if ( intbits < 1 ) {
        fprintf(stderr,"error: intbits must be at least 1 (sign bit)\n");
        return -1;
    } else if ( totalbits != 8 && totalbits != 16 && totalbits != 32) {
        fprintf(stderr,"error: intbits (%u) + fracbits (%u) must equal 8|16|32\n",intbits,fracbits);
        return -1;
    }

    // mangling...
    if (arch == ARCH_PPC) {
        comment_char = ';';
        strcpy(name_mangler,"_");
        strcpy(archname,"ppc");
    } else if (arch == ARCH_X86) {
        comment_char = '#';
        strcpy(name_mangler,"");
        strcpy(archname,"x86");
    } else if (arch == ARCH_INTELMAC) {
        comment_char = '#';
        strcpy(name_mangler,"_");
        strcpy(archname,"intelmac");
    } else {
        comment_char = '#';
        strcpy(name_mangler,"");
        strcpy(archname,"[unknown]");
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

    fprintf(fid,"%c \n", comment_char);
    fprintf(fid,"%c auto-generated file : do not edit\n", comment_char);
    fprintf(fid,"%c \n", comment_char);
    fprintf(fid,"%c   target architecture : %s\n", comment_char, archname);
    fprintf(fid,"    .text\n");
    fprintf(fid,"    .set qint,  %3u   %c intbits\n",intbits,comment_char);
    fprintf(fid,"    .set qfrac, %3u   %c fracbits\n",fracbits,comment_char);
    fprintf(fid,"    .globl %s%s_mul\n", name_mangler,typename);
    fprintf(fid,"\n");
    fprintf(fid,"%c multiplication\n", comment_char);
    fprintf(fid,"%s%s_mul:\n", name_mangler,typename);
    fprintf(fid,".include \"qtype.mul.%s.s\"\n", archname);

    // close the file (if not printing to stdout)
    if (outputfile_set)
        fclose(fid);

    return 0;
}

