#include <benchmark/benchmark.h>
#include <iostream>
#include <cassert>

#include "hwy/aligned_allocator.h"
#include "populationCount.hpp"

#define BENCHMARK_REPETITIONS 10

const uint32_t EXPECTED_RESULT = Lanes(d) * 32;

#ifndef SVE
#ifndef AVX512
static void BM_AVX2_Extract_Popcnt_64(benchmark::State& state) {

    uint32_t vec[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec[i] = 0xFFFFFFFF;
    }
    __m256i v_t = _mm256_loadu_si256((__m256i*) &vec[0]);

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = avx2_extract_popcnt_64(v_t);
        }
    }
    assert(result == 256);
}
BENCHMARK(BM_AVX2_Extract_Popcnt_64)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif  // AVX2
#endif	// SVE

#ifdef AVX512
static void BM_AVX512_Popcnt_Reduce_32(benchmark::State& state) {

    uint32_t vec[16]; 
    for(size_t i = 0; i < 16; i++) {
        vec[i] = 0xFFFFFFFF;
    }
    __m512i v_t = _mm512_loadu_si512((__m512i*) &vec[0]);

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = avx512_popcnt_reduce_32(v_t);
        }
    }
    assert(result == 512);
}
BENCHMARK(BM_AVX512_Popcnt_Reduce_32)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });
#endif  // AVX512

static void BM_Highway_Popcnt_SumOfLanes(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }

    V v_t = LoadU(d, &vec[0]); 

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = highway_popcnt_SumOfLanes(v_t);
        }
    }
    assert(result == EXPECTED_RESULT);
}
BENCHMARK(BM_Highway_Popcnt_SumOfLanes)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });

static void BM_Highway_Extract_Popcnt_32(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }

    V v_t = LoadU(d, &vec[0]); 

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = highway_extract_popcnt_32(v_t);
        }
    }
    assert(result == EXPECTED_RESULT);
}
BENCHMARK(BM_Highway_Extract_Popcnt_32)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });


static void BM_Highway_Extract_Popcnt_64(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }
    
    // std::cout << TargetName(HWY_TARGET) << std::endl;
    V v_t = LoadU(d, &vec[0]); 

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = highway_extract_popcnt_64(v_t);
        }
    }
    assert(result == EXPECTED_RESULT);
}
BENCHMARK(BM_Highway_Extract_Popcnt_64)
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });



static void BM_Highway_Store_Popcnt(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }

    V v_t = LoadU(d, &vec[0]); 

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = highway_store_popcnt(v_t);
        }
    }
    assert(result == EXPECTED_RESULT);
}
BENCHMARK(BM_Highway_Store_Popcnt)    
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });;

/*
static void BM_Highway_Extract(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }

    V input = LoadU(d, &vec[0]);

    const ScalableTag<uint64_t> d; 
    uint64_t t = 0; 
    auto vector = BitCast(d,input);

    uint64_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            for(int i = 0; i  < Lanes(d); i += 4) {
                result += ExtractLane(vector, i);
                result += ExtractLane(vector, i + 1);
                result += ExtractLane(vector, i + 2);
                result += ExtractLane(vector, i + 3);
                benchmark::DoNotOptimize(result);
            }
        }
    }
}
BENCHMARK(BM_Highway_Extract)    
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });;


static void BM_Highway_SumOfLanes(benchmark::State& state) {

    uint32_t vec[Lanes(d)]; 
    for(size_t i = 0; i < Lanes(d); i++) {
        vec[i] = 0xFFFFFFFF;
    }

    V input = LoadU(d, &vec[0]);

    uint32_t result = 0; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = GetLane(SumOfLanes(d, input)); 
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_Highway_SumOfLanes)    
    ->Repetitions(BENCHMARK_REPETITIONS)
    ->DisplayAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
    })
    ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
    });;
*/
BENCHMARK_MAIN();
