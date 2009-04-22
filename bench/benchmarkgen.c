// benchmarkgen.c
// 
// Benchmark generator script

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool parse_filename(char * _filename, char * _basename);

typedef struct {
    char filename[256];
    char basename[256];
} benchmark_t;

int main(int argc, char*argv[]) {
    // allocate memory for all possible benchmarks
    benchmark_t benchmarks[argc-1];

    int i;
    unsigned int n = 0;
    char tmp_basename[256];
    bool filename_valid;
    for (i=1; i<argc; i++) {
        filename_valid = parse_filename(argv[i], tmp_basename);

        if (filename_valid) {
            strncpy(benchmarks[n].filename, argv[i], 256);
            strncpy(benchmarks[n].basename, tmp_basename, 256);
            n++;
        }
    }

    // print header
    printf("// auto-generated file, do not edit\n\n");
    
    printf("#ifndef __LIQUIDFPM_BENCHMARK_INCLUDE_H__\n");
    printf("#define __LIQUIDFPM_BENCHMARK_INCLUDE_H__\n\n");

    printf("// header files from which this file was generated\n");
    for (i=0; i<n; i++) {
        printf("#include \"%s\"\n", benchmarks[i].filename);
    }
    printf("\n");

    printf("// number of benchmarks\n");
    printf("#define NUM_BENCHMARKS %u\n\n", n);

    printf("bench_t benchmarks[NUM_BENCHMARKS] = {\n");
    for (i=0; i<n; i++) {
        printf("    {%4u, &precision_%s, &benchmark_%s, \"%s\"}",
            i,
            benchmarks[i].basename,
            benchmarks[i].basename,
            benchmarks[i].basename);
        if (i<n)
            printf(",");
        printf("\n");
    }
    printf("};\n\n");

    printf("#endif // __LIQUIDFPM_BENCHMARK_INCLUDE_H__\n\n");

    return 0;
}

bool parse_filename(char * _filename, char * _basename)
{
    char substr[256];
    char * subptr;
    int i0, i1;
    char pathsep = '/';
    const char tag[] = "_benchmark.h";

    // try to strip out path: find rightmost occurrence of pathsep
    printf("%s\n", _filename);
    subptr = strrchr(_filename, pathsep);   // obtain index of last pathsep
    if (subptr == NULL) {
        printf("path delimiter not found\n");
        i0 = 0;
    } else {
        i0 = subptr - _filename + 1;
    }
    printf("  i0 : %d\n", i0);

    // try to strip out tag: "_benchmark.h"
    subptr = strrchr( _filename, tag[0] );
    if (subptr == NULL) {
        printf("  tag not found\n");
        return false;
    } else {
        i1 = subptr - _filename;
        printf("  i1 : %d\n", i1);
    }

    // ensure the last occurrence of tag is not in the path name
    if (i0 >= i1) {
        printf("invalid path name\n");
        return false;
    }
    
    // ensure tag is valid
    strncpy(substr,&_filename[i1],256);
    printf("  comparing %s with %s\n", tag, substr);
    if (strncmp(tag,&_filename[i1],strlen(tag)) != 0 ) {
        printf("  invalid tag (comparison failed)\n");
        return false;
    } else {
        printf("  comparison passed!\n");
    }

    // copy base name
    strncpy( substr, _filename+i0, i1-i0 );
    // add null character to end
    substr[i1-i0] = '\0';
    printf("base: \"%s\"\n", substr);

    strncpy(_basename, substr, 256);

    printf("\n\n");
    return true;
}

