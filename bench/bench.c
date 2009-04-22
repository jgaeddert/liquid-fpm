// bench.c
//
// Test fixed-point math library (precision, speed)
//

// default include header files
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/resource.h>

// define precision function pointer
typedef void(*precision_function_t) (
    unsigned int _res,
    float * _rmse,
    float * _abse,
    FILE * _precision_file);

// define benchmark function pointer
typedef void(*benchmark_function_t) (
    struct rusage * _start,
    struct rusage * _finish,
    unsigned long int * _num_trials);

// define basic structure for storing results
typedef struct {
    unsigned int id;

    // APIs
    precision_function_t precision_api;
    benchmark_function_t benchmark_api;

    const char * name;

    // precision
    float rms_error;
    float abs_error;
#if 0
    float * input_val;      // input value
    float * output_true;    // true (correct) output
    float * output_actual;  // actual output
#endif

    // speed
    unsigned long int num_trials;
    float extime;
    float rate;
    float cycles_per_trial;
} bench_t;

// include auto-generated benchmark header
//
// defines the following symbols...
//
#include "../benchmark_include.h"

double calculate_execution_time(struct rusage _start, struct rusage _finish);

int main() {
    //
    struct rusage t0;
    struct rusage t1;
    unsigned long int num_trials = 100000;
    double extime;
    unsigned int resolution = 256;
    float rmse;
    float abse;
    
    unsigned int i;

    FILE * fid = fopen("precision.m", "w");

    // run benchmarks
    for (i=0; i<NUM_BENCHMARKS; i++) {
        // run precision test
        benchmarks[i].precision_api(resolution,&rmse,&abse,fid);

        // run speed test
        benchmarks[i].benchmark_api(&t0,&t1,&num_trials);

        // compile results
        extime = calculate_execution_time(t0,t1);

        // print results
        printf("    %-20s : %12.6f s : %12.8f\n", benchmarks[i].name, extime, rmse);
    }

    fclose(fid);

    printf("done.\n");

    return 0;
}

double calculate_execution_time(struct rusage _start, struct rusage _finish)
{
    return _finish.ru_utime.tv_sec - _start.ru_utime.tv_sec
        + 1e-6*(_finish.ru_utime.tv_usec - _start.ru_utime.tv_usec)
        + _finish.ru_stime.tv_sec - _start.ru_stime.tv_sec
        + 1e-6*(_finish.ru_stime.tv_usec - _start.ru_stime.tv_usec);
}


