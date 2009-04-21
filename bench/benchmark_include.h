//
//
//

#ifndef __LIQUIDFPM_BENCHMARK_INCLUDE_H__
#define __LIQUIDFPM_BENCHMARK_INCLUDE_H__

// header files from which this file was generated
#include "bench/log2_benchmark.h"
#include "bench/sqrt_benchmark.h"
#include "bench/sin_benchmark.h"
#include "bench/atan2_benchmark.h"

#define NUM_BENCHMARKS  4

bench_t benchmarks[NUM_BENCHMARKS] = {
    {0, &precision_log2,    &benchmark_log2,    "log2"  },
    {1, &precision_sqrt,    &benchmark_sqrt,    "sqrt"  },
    {2, &precision_sin,     &benchmark_sin,     "sin"   },
    {3, &precision_atan2,   &benchmark_atan2,   "atan2" }
};

#endif // __LIQUIDFPM_BENCHMARK_INCLUDE_H__
