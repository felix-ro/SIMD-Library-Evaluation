#include <hwy/highway.h>

#ifndef SVE
#include <immintrin.h>
#endif

using namespace hwy;
using namespace HWY_NAMESPACE;

const ScalableTag<uint32_t> d;
using V = decltype(Zero(d));  

HWY_ATTR uint32_t highway_popcnt_SumOfLanes(V v_t);
HWY_ATTR uint32_t highway_extract_popcnt_32(V v_t);
HWY_ATTR uint32_t highway_extract_popcnt_64(V input);
HWY_ATTR uint32_t highway_store_popcnt(V v_t);

#ifndef SVE
uint32_t avx2_extract_popcnt_64(__m256i v_t);
#endif

#ifdef AVX512
uint32_t avx512_popcnt_reduce_32(__m512i v_t);
#endif
