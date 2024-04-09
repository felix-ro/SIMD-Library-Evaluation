#include <iostream>
#include <chrono>
#include <benchmark/benchmark.h>
#include "hwy/aligned_allocator.h"
#include <hwy/highway.h>
#include "hwy/nanobenchmark.h"  // Unpredictable1
#include <numeric>  // iota
#include <Vc/Vc>
#include <simdpp/simd.h>

#include "dotProduct.hpp"
#include "dotProductHighway.hpp"
#include "../utils/utils.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration;

using namespace std;

const size_t length = 2048;

static void BM_Dot_Product(benchmark::State& state) {

    float a[length]; 
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product); 


static void BM_Dot_Product_Unrolled(benchmark::State& state) {

    float a[length]; 
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_unrolled(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_Unrolled); 


static void BM_Dot_Product_Modified(benchmark::State& state) {

    float a[length]; 
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_modified(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_Modified); 


static void BM_Dot_Product_OpenMP(benchmark::State& state) {

    float a[length]; 
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_openMP(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_OpenMP);


static void BM_Dot_Product_AVX2(benchmark::State& state) {

    __attribute__((aligned(32))) float a[length];
    __attribute__((aligned(32))) float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_AVX2(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_AVX2);



static void BM_Dot_Product_AVX2_unrolled(benchmark::State& state) {

    __attribute__((aligned(32))) float a[length];
    __attribute__((aligned(32))) float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_AVX2_unrolled(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_AVX2_unrolled); 


static void BM_Dot_Product_Highway(benchmark::State& state) {

    hwy::AlignedFreeUniquePtr<float []> a = hwy::AllocateAligned<float>(length * 2);
    float * b = &a[0] + length;

    //const float init = static_cast<float>(hwy::Unpredictable1());
    //std::iota(a.get(), a.get() + length, init);
    //std::iota(b, b + length, init);

    fillFloatArrayRandom(a.get(), length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        highway_dot_product(&a[0], b, length);
    }
}
BENCHMARK(BM_Dot_Product_Highway);


static void BM_Dot_Product_Highway_Unrolled(benchmark::State& state) {

    hwy::AlignedFreeUniquePtr<float []> a = hwy::AllocateAligned<float>(length * 2);
    float * b = &a[0] + length;

    fillFloatArrayRandom(a.get(), length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        highway_dot_product_unrolled(&a[0], b, length);
    }
}
BENCHMARK(BM_Dot_Product_Highway_Unrolled);


static void BM_Dot_Product_Vc(benchmark::State& state) {

    float a[length];
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_vc(a, b, length);
        //benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Dot_Product_Vc);


static void BM_Dot_Product_Vc_Unrolled(benchmark::State& state) {

    float a[length];
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_vc_unrolled(a, b, length);
        //benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Dot_Product_Vc_Unrolled);


static void BM_Dot_Product_Libsimdpp(benchmark::State& state) {
    
    simdpp::aligned_allocator<float, 64> allocator; 
    float * a = allocator.allocate(length);
    float * b = allocator.allocate(length); 

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_libsimdpp(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_Libsimdpp);


static void BM_Dot_Product_Libsimdpp_Unrolled(benchmark::State& state) {

    simdpp::aligned_allocator<float, 64> allocator; 
    float * a = allocator.allocate(length);
    float * b = allocator.allocate(length); 

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_libsimdpp_unrolled(a, b, length);
    }
}
BENCHMARK(BM_Dot_Product_Libsimdpp_Unrolled);


static void BM_Dot_Product_Pure_Simd(benchmark::State& state) {
    float a[length];
    float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_pure_simd(a, b, length);
        //benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Dot_Product_Pure_Simd);


#ifdef AVX512
static void BM_Dot_Product_AVX512(benchmark::State& state) {
    __attribute__((aligned(64))) float a[length];
    __attribute__((aligned(64))) float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_avx512(a, b, length);
        //benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Dot_Product_AVX512);


static void BM_Dot_Product_AVX512_Unrolled(benchmark::State& state) {
    __attribute__((aligned(64))) float a[length];
    __attribute__((aligned(64))) float b[length];

    fillFloatArrayRandom(a, length);
    fillFloatArrayRandom(b, length);

    for (auto _ : state) {
        dot_product_avx512_unrolled(a, b, length);
        //benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Dot_Product_AVX512_Unrolled);
#endif	// AVX512


BENCHMARK_MAIN(); 
