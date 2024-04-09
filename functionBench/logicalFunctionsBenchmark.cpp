#include <benchmark/benchmark.h>
#include <iostream>

#include "hwy/aligned_allocator.h"
#include "logicalFunctions.hpp"


static void BM_Logical_AND_AVX(benchmark::State& state) {
    
    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }
    __m256i v_a = _mm256_loadu_si256((__m256i*) &vec_a[0]);
    __m256i v_b = _mm256_loadu_si256((__m256i*) &vec_b[0]);


    __m256i result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalAndTestAVX(v_a, v_b);
            //DoNotOptemize(result); 
        }
    }
}
BENCHMARK(BM_Logical_AND_AVX); 

    // ->Repetitions(10)
    // ->DisplayAggregatesOnly(true)
    // ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    // return *(std::max_element(std::begin(v), std::end(v)));
    // })
    // ->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    // return *(std::min_element(std::begin(v), std::end(v)));
    // });

static void BM_Logical_AND_Highway(benchmark::State& state) {

    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }

    V v_a = LoadU(d, &vec_a[0]);
    V v_b = LoadU(d, &vec_b[0]); 

    V result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalAndTestHighway(v_a, v_b);
        }
    }
}
BENCHMARK(BM_Logical_AND_Highway);

static void BM_Logical_Or_AVX(benchmark::State& state) {
    
    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }
    __m256i v_a = _mm256_loadu_si256((__m256i*) &vec_a[0]);
    __m256i v_b = _mm256_loadu_si256((__m256i*) &vec_b[0]);


    __m256i result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalOrTestAVX(v_a, v_b);
            //DoNotOptemize(result); 
        }
    }
}
BENCHMARK(BM_Logical_Or_AVX);

static void BM_Logical_Or_Highway(benchmark::State& state) {

    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }

    V v_a = LoadU(d, &vec_a[0]);
    V v_b = LoadU(d, &vec_b[0]); 

    V result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalOrTestHighway(v_a, v_b);
        }
    }
}
BENCHMARK(BM_Logical_Or_Highway);

static void BM_Logical_Xor_AVX(benchmark::State& state) {
    
    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }
    __m256i v_a = _mm256_loadu_si256((__m256i*) &vec_a[0]);
    __m256i v_b = _mm256_loadu_si256((__m256i*) &vec_b[0]);


    __m256i result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalXorTestAVX(v_a, v_b);
            //DoNotOptemize(result); 
        }
    }
}
BENCHMARK(BM_Logical_Xor_AVX);

static void BM_Logical_Xor_Highway(benchmark::State& state) {

    uint32_t vec_a[8];
    uint32_t vec_b[8]; 
    for(size_t i = 0; i < 8; i++) {
        vec_a[i] = 0xFFFFFFFF;
        vec_b[i] = 0x00FF00FF;
    }

    V v_a = LoadU(d, &vec_a[0]);
    V v_b = LoadU(d, &vec_b[0]); 

    V result; 
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = logicalXorTestHighway(v_a, v_b);
        }
    }
}
BENCHMARK(BM_Logical_Xor_Highway);

static void BM_Set_Function_AVX(benchmark::State& state) {

    uint32_t start = 32; 

    __m256i result;
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = setFunctionAVX(start);
        }
    }
}
BENCHMARK(BM_Set_Function_AVX);

static void BM_Set_Function_Highway(benchmark::State& state) {

    uint32_t start = 32; 

    V result;
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = setFunctionHighway(start);
        }
    }
}
BENCHMARK(BM_Set_Function_Highway);

static void BM_Masked_Load_Highway(benchmark::State& state) {

    uint32_t arr[8]; 
    for(size_t i = 0; i < 8; i++) {
        arr[i] = i;
    }

    V result;
    for (auto _ : state) {
        for(size_t i = 0; i < 10000; i++) {
            result = maskedLoadHighway(&arr[0]);
        }
    }
}
BENCHMARK(BM_Masked_Load_Highway);

BENCHMARK_MAIN();
