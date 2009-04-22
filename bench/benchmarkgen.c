// benchmarkgen.c
// 
// Benchmark generator script

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_filename(char * _filename);

int main(int argc, char*argv[]) {
    // 
    int i;
    for (i=0; i<argc; i++) {
        //printf("filename : %s\n", argv[i]);
        parse_filename(argv[i]);
    }

    printf("done.\n");
    return 0;
}

void parse_filename(char * _filename)
{
    int results=0;
    char substr[256];
    char * subptr;
    int i0, i1;
    const char tag[] = "_benchmark.h";

    // try to strip out path: find rightmost occurrence of pathsep
    printf("%s\n", _filename);
    char pathsep = '/';
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
        return;
    } else {
        i1 = subptr - _filename;
        printf("  i1 : %d\n", i1);
    }

    // ensure the last occurrence of tag is not in the path name
    if (i0 >= i1) {
        printf("invalid path name\n");
        return 0;
    }
    
    // ensure tag is valid
    strncpy(substr,&_filename[i1],256);
    printf("  comparing %s with %s\n", tag, substr);
    if (strncmp(tag,&_filename[i1],strlen(tag)) != 0 ) {
        printf("  invalid tag (comparison failed)\n");
        return;
    } else {
        printf("  comparison passed!\n");
    }

    // copy base name
    strncpy( substr, _filename+i0, i1-i0 );
    // add null character
    substr[i1-i0] = '\0';
    printf("base: \"%s\"\n", substr);

    printf("\n\n");
}

