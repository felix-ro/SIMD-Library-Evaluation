#include <benchmark/benchmark.h>
#include "hwy/aligned_allocator.h"

#ifndef SVE
#include <simdpp/simd.h>
#endif

#include "mandelbrot.hpp"
#include "nsimdMandelbrot.hpp"
#include "nsimdBaseMandelbrot.hpp"
#include "simdeMandelbrot.hpp"
#include "mandelbrotScalar.hpp"
#include "../utils/utils.hpp"

#define BENCHMARK_REPETITIONS 10

const static size_t width = 1024;
const static size_t height = 1024;
const static float xBegin = -1.5f;
const static float xEnd = 0.5f;
const static float yBegin = -1.5f; 
const static float yEnd = 1.5f; 

static void BM_Mandelbrot_Scalar(benchmark::State& state) {
    float image[width * height]; 

    for (auto _ : state) {
        mandelbrot_scalar(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_Scalar)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });

static void BM_Mandelbrot_Scalar_ComplexClass(benchmark::State& state) {
    int image[width * height]; 

    for (auto _ : state) {
        mandelbrot_scalar_complexClass(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
/*
BENCHMARK(BM_Mandelbrot_Scalar_ComplexClass)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
*/

static void BM_Mandelbrot_AutoVec(benchmark::State& state) {
    float image[width * height]; 

    for (auto _ : state) {
        mandelbrot_autoVec(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_AutoVec)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });


static void BM_Mandelbrot_AutoVec_ComplexClass(benchmark::State& state) {
    int image[width * height]; 

    for (auto _ : state) {
        mandelbrot_autoVec_complexClass(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
/*
BENCHMARK(BM_Mandelbrot_AutoVec_ComplexClass)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
*/

static void BM_Mandelbrot_OpenMP(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height]; 

    for (auto _ : state) {
        mandelbrot_openMP(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_OpenMP)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });

#ifndef AVX512
#ifndef NEON
#ifndef SVE
static void BM_Mandelbrot_AVX2(benchmark::State& state) {
    __attribute__((aligned(32))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_avx2(xBegin, xEnd, yBegin, yEnd, width, height, image); 
    }
}
BENCHMARK(BM_Mandelbrot_AVX2)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif	// SVE
#endif 	// NEON
#endif 	// AVX512


static void BM_Mandelbrot_Highway(benchmark::State& state) {
    hwy::AlignedFreeUniquePtr<float []> image = hwy::AllocateAligned<float>(width * height);

    for (auto _ : state) {
        mandelbrot_highway(xBegin, xEnd, yBegin, yEnd, width, height, &image[0]);
    }
}
BENCHMARK(BM_Mandelbrot_Highway)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });


#ifndef AVX512
#ifndef NEON
#ifndef SVE
static void BM_Mandelbrot_Vc(benchmark::State& state) {
    float image[width * height];

    for (auto _ : state) {
        mandelbrot_vc(xBegin, xEnd, yBegin, yEnd, width, height, image); 
    }
}
BENCHMARK(BM_Mandelbrot_Vc)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif	// SVE
#endif 	// NEON
#endif 	// AVX512


#ifndef SVE
static void BM_Mandelbrot_Libsimdpp(benchmark::State& state) {
#ifndef AVX512
    simdpp::aligned_allocator<float, 32> allocator;
#else
    simdpp::aligned_allocator<float, 64> allocator;
#endif    
    float * image = allocator.allocate(width * height); 

    for (auto _ : state) {
        mandelbrot_libsimdpp(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_Libsimdpp)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif


static void BM_Mandelbrot_Pure_Simd(benchmark::State& state) {
    float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_pure_simd(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_Pure_Simd)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });

#ifndef SVE
static void BM_Mandelbrot_NSIMD(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_nsimd(xBegin, xEnd, yBegin, yEnd, width, height, image); 
    }
}
BENCHMARK(BM_Mandelbrot_NSIMD)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif


#ifndef SVE
static void BM_Mandelbrot_NSIMD_BASE(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_nsimdBase(xBegin, xEnd, yBegin, yEnd, width, height, image); 
    }
}
BENCHMARK(BM_Mandelbrot_NSIMD_BASE)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif


static void BM_Mandelbrot_SIMDe(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_simde_avx2(xBegin, xEnd, yBegin, yEnd, width, height, image); 
    }
}
BENCHMARK(BM_Mandelbrot_SIMDe)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });


#ifndef NEON
#ifndef SVE
#ifdef AVX512
static void BM_Mandelbrot_AVX512(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_avx512(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_AVX512)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif	// AVX512
#endif	// SVE
#endif	// NEON 


#ifdef NEON
static void BM_Mandelbrot_NEON(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_neon(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_NEON)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif


#ifdef SVE
static void BM_Mandelbrot_SVE(benchmark::State& state) {
    __attribute__((aligned(64))) float image[width * height] = {0};

    for (auto _ : state) {
        mandelbrot_sve(xBegin, xEnd, yBegin, yEnd, width, height, image);
    }
}
BENCHMARK(BM_Mandelbrot_SVE)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif 	// SVE

BENCHMARK_MAIN();
