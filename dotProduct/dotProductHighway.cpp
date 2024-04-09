#include "hwy/aligned_allocator.h"
#include <hwy/highway.h>
#include "hwy/nanobenchmark.h"  // Unpredictable1

#include "dotProductHighway.hpp"

//#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <numeric>  // iota

using namespace hwy;
using namespace HWY_NAMESPACE;


HWY_ATTR float highway_dot_product(const float* const HWY_RESTRICT pa, 
                          const float* const HWY_RESTRICT pb, size_t numItems) {
  const ScalableTag<float> d;
  const size_t N = Lanes(d);
  using V = decltype(Zero(d));

  V sum = Zero(d);
  for (size_t i = 0; i < numItems; i += N) {
    const auto a = Load(d, pa + i);
    const auto b = Load(d, pb + i);
    sum = MulAdd(a, b, sum);
  }

  return GetLane(SumOfLanes(d, sum));;
}

float highway_dot_product_unrolled(const float* const HWY_RESTRICT pa, 
                          const float* const HWY_RESTRICT pb, size_t numItems) {
  const ScalableTag<float> d;
  const size_t N = Lanes(d);
  using V = decltype(Zero(d));

  V sum0 = Zero(d);
  V sum1 = Zero(d);
  V sum2 = Zero(d);
  V sum3 = Zero(d);
  for (size_t i = 0; i < numItems; i += 4 * N) {
    const auto a0 = Load(d, pa + i + 0 * N);
    const auto b0 = Load(d, pb + i + 0 * N);
    sum0 = MulAdd(a0, b0, sum0);
    const auto a1 = Load(d, pa + i + 1 * N);
    const auto b1 = Load(d, pb + i + 1 * N);
    sum1 = MulAdd(a1, b1, sum1);
    const auto a2 = Load(d, pa + i + 2 * N);
    const auto b2 = Load(d, pb + i + 2 * N);
    sum2 = MulAdd(a2, b2, sum2);
    const auto a3 = Load(d, pa + i + 3 * N);
    const auto b3 = Load(d, pb + i + 3 * N);
    sum3 = MulAdd(a3, b3, sum3);
  }
  
  // Reduction tree: sum of all accumulators by pairs into sum0.
  sum0 = Add(sum0, sum1);
  sum2 = Add(sum2, sum3);
  sum0 = Add(sum0, sum2);
  float result = GetLane(SumOfLanes(d, sum0));
  return result;
}

/*
int main() {
  size_t num_items = 131072; 
  AlignedFreeUniquePtr<float []> a = AllocateAligned<float>(num_items * 2);
  float * b = &a[0] + num_items;

  const float init = static_cast<float>(Unpredictable1());
  std::iota(a.get(), a.get() + num_items, init);
  std::iota(b, b + num_items, init);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto startTime = high_resolution_clock::now();
  float result = highway_dot_product(&a[0], b, num_items);
  auto endTime = high_resolution_clock::now(); 
  
  printf("Result: %f\n", result);
  duration<double, std::milli> ms_double = endTime - startTime;

  std::cout << ms_double.count() << "ms\n";

  return 0;
}


static void BM_TestHighway(benchmark::State& state) {
  size_t num_items = 131072; 
  AlignedFreeUniquePtr<float []> a = AllocateAligned<float>(num_items * 2);
  float * b = &a[0] + num_items;

  const float init = static_cast<float>(Unpredictable1());
  std::iota(a.get(), a.get() + num_items, init);
  std::iota(b, b + num_items, init);

  for (auto _ : state) {
    float result = highway_dot_product(&a[0], b, num_items);
    benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(BM_TestHighway);

BENCHMARK_MAIN(); 
*/