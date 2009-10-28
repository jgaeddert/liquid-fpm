// autotestgen.c
// 
// Autotest generator script

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool parse_filename(char * _filename, char * _basename);

typedef struct {
    char filename[256];     // e.g. "/path/to/mytest_autotest.h"
    char basename[256];     //   >> "mytest"
} autotest_t;

int main(int argc, char*argv[]) {
    // allocate memory for all possible autotests
    autotest_t autotests[argc-1];

    int i;
    unsigned int n = 0;
    char tmp_basename[256];
    bool filename_valid;
    for (i=1; i<argc; i++) {
        filename_valid = parse_filename(argv[i], tmp_basename);

        if (filename_valid) {
            strncpy(autotests[n].filename, argv[i], 256);
            strncpy(autotests[n].basename, tmp_basename, 256);
            n++;
        }
    }

    // print header
    printf("// auto-generated file, do not edit\n\n");
    
    printf("#ifndef __LIQUIDFPM_AUTOTEST_INCLUDE_H__\n");
    printf("#define __LIQUIDFPM_AUTOTEST_INCLUDE_H__\n\n");

    printf("// header files from which this file was generated\n");
    for (i=0; i<n; i++) {
        printf("#include \"%s\"\n", autotests[i].filename);
    }
    printf("\n");

    printf("// number of autotests\n");
    printf("#define NUM_AUTOTESTS %u\n\n", n);

    printf("struct autotest_s autotests[NUM_AUTOTESTS] = {\n");
    for (i=0; i<n; i++) {
        printf("    {%4u, &%s_autotest, \"%s\"}",
            i,
            autotests[i].basename,
            autotests[i].basename);
        if (i<n-1)
            printf(",");
        printf("\n");
    }
    printf("};\n\n");

    printf("#define NUM_PACKAGES 1\n");
    printf("struct package_s packages[NUM_PACKAGES] = {\n");
    printf("    {0,0,%u,\"all\"}\n", n);
    printf("};\n");

    printf("#endif // __LIQUIDFPM_AUTOTEST_INCLUDE_H__\n\n");

    return 0;
}

bool parse_filename(char * _filename, char * _basename)
{
    char substr[256];
    char * subptr;
    int i0, i1;
    char pathsep = '/';
    const char tag[] = "_autotest.h";

    // try to strip out path: find rightmost occurrence of pathsep
    //printf("%s\n", _filename);
    subptr = strrchr(_filename, pathsep);   // obtain index of last pathsep
    if (subptr == NULL) {
        //printf("path delimiter not found\n");
        i0 = 0;
    } else {
        i0 = subptr - _filename + 1;
    }
    //printf("  i0 : %d\n", i0);

    // try to strip out tag: "_autotest.h"
    subptr = strrchr( _filename, tag[0] );
    if (subptr == NULL) {
        //printf("  tag not found\n");
        return false;
    } else {
        i1 = subptr - _filename;
        //printf("  i1 : %d\n", i1);
    }

    // ensure the last occurrence of tag is not in the path name
    if (i0 >= i1) {
        //printf("invalid path name\n");
        return false;
    }
    
    // ensure tag is valid
    //strncpy(substr,&_filename[i1],256);
    //printf("  comparing %s with %s\n", tag, substr);
    if (strncmp(tag,&_filename[i1],strlen(tag)) != 0 ) {
        //printf("  invalid tag (comparison failed)\n");
        return false;
    } else {
        //printf("  comparison passed!\n");
    }

    // copy base name
    strncpy( substr, _filename+i0, i1-i0 );
    // add null character to end
    substr[i1-i0] = '\0';
    //printf("base: \"%s\"\n", substr);

    strncpy(_basename, substr, 256);

    //printf("\n\n");
    return true;
}

