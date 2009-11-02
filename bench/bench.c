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
    float * _abse);

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

// globals
unsigned long int num_trials = 1<<16;
float cpu_clock = 1.0f; // cpu clock speed (Hz)

// forward method declarations
double calculate_execution_time(struct rusage _start, struct rusage _finish);
char convert_units(float * _v);
void estimate_cpu_clock(void);

int main() {
    //
    struct rusage t0;
    struct rusage t1;
    unsigned long int n;
    double extime;
    double cycles_per_trial;
    unsigned int resolution = 256;
    float rmse;
    float maxe;

    estimate_cpu_clock();
    num_trials = (unsigned long int) ( cpu_clock / 1000 );
    num_trials = num_trials < 256 ? 256 : num_trials;
    
    unsigned int i;

    // run benchmarks
    for (i=0; i<NUM_BENCHMARKS; i++) {
        // run precision test
        benchmarks[i].precision_api(resolution,&rmse,&maxe);

        // run speed test
        n = num_trials;
        benchmarks[i].benchmark_api(&t0,&t1,&n);

        // compile results
        extime = calculate_execution_time(t0,t1);
        cycles_per_trial = extime * cpu_clock / (n);

        // print results
        printf("    %-20s : %10.6f s (%7.2f cycles/tr) : %10.8f rms, %10.8f max\n",
            benchmarks[i].name,
            extime,
            cycles_per_trial,
            rmse,
            maxe);
    }

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


// convert raw value into metric units,
//   example: "0.01397s" -> "13.97 ms"
char convert_units(float * _v)
{
    char unit;
    if (*_v < 1e-9)     {   (*_v) *= 1e12;  unit = 'p';}
    else if (*_v < 1e-6){   (*_v) *= 1e9;   unit = 'n';}
    else if (*_v < 1e-3){   (*_v) *= 1e6;   unit = 'u';}
    else if (*_v < 1e+0){   (*_v) *= 1e3;   unit = 'm';}
    else if (*_v < 1e3) {   (*_v) *= 1e+0;  unit = ' ';}
    else if (*_v < 1e6) {   (*_v) *= 1e-3;  unit = 'k';}
    else if (*_v < 1e9) {   (*_v) *= 1e-6;  unit = 'M';}
    else if (*_v < 1e12){   (*_v) *= 1e-9;  unit = 'G';}
    else                {   (*_v) *= 1e-12; unit = 'T';}

    return unit;
}


// run basic benchmark to estimate CPU clock frequency
void estimate_cpu_clock(void)
{
    printf("  estimating cpu clock frequency...\n");
    unsigned long int i, n = 1<<4;
    struct rusage start, finish;
    double extime;
    
    do {
        // trials
        n <<= 1;
        unsigned int x=0;
        getrusage(RUSAGE_SELF, &start);
        for (i=0; i<n; i++) {
            // perform mindless task
            x <<= 1;
            x |= 1;
            x &= 0xff;
            x ^= 0xff;
        }
        getrusage(RUSAGE_SELF, &finish);

        extime = calculate_execution_time(start, finish);
    } while (extime < 0.5 && n < (1<<28));

    // estimate cpu clock frequency
    cpu_clock = 23.9 * n / extime;

    printf("  performed %ld trials in %5.1f ms\n", n, extime * 1e3);
    
    float clock_format = cpu_clock;
    char clock_units = convert_units(&clock_format);
    printf("  estimated clock speed: %7.3f %cHz\n", clock_format, clock_units);
}

