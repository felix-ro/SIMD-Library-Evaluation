#include <benchmark/benchmark.h>
#include <hwy/aligned_allocator.h>
#include <hwy/highway.h>

#ifndef SVE
#include <immintrin.h>
#endif

#include <iostream>
#include <cassert>

#define BENCHMARK_REPETITIONS 1

using namespace hwy;
using namespace HWY_NAMESPACE;

const ScalableTag<uint32_t> d;
using V = decltype(Zero(d));  

#ifdef AVX512
static void BM_AVX512_Popcnt_32(benchmark::State& state) {
    const size_t length = 1024;
    __attribute__((aligned(64))) uint32_t array[length];
    for (size_t i = 0; i < length; i++) {
        array[i] = 0xFFFFFFFF; 
    }  

    __m512i sum;
    for (auto _ : state) {
        sum = _mm512_setzero_epi32();
        for (size_t i = 0; i < length; i += 16) {
            __m512i vec = _mm512_load_si512((__m512i*) &array[i]);
            vec = _mm512_popcnt_epi32(vec); 
            sum = _mm512_add_epi32(sum, vec);
        }
    }
    int result = _mm512_reduce_add_epi32(sum);
    assert(result == (1024*32));
}
BENCHMARK(BM_AVX512_Popcnt_32)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif  // AVX512

static void BM_Highway_Popcnt_32(benchmark::State& state) {
    const size_t length = 1024;
    __attribute__((aligned(64))) uint32_t array[length]; 
    for (size_t i = 0; i < length; i++) {
        array[i] = 0xFFFFFFFF; 
    } 

    V sum;
    for (auto _ : state) {
        sum = Zero(d);
        for (size_t i = 0; i < length; i += Lanes(d)) {
            V vec = Load(d, &array[i]);
            vec = PopulationCount(vec); 
            sum = Add(sum, vec);
        }
    } 
    int result = GetLane(SumOfLanes(d, sum));
    //std::cout << result << std::endl; 
    assert(result == (1024*32));
}
BENCHMARK(BM_Highway_Popcnt_32)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });

BENCHMARK_MAIN();

