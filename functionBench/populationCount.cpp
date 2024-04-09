#include "populationCount.hpp"

HWY_ATTR uint32_t highway_popcnt_SumOfLanes(V input) {
    return GetLane(SumOfLanes(d, PopulationCount(input)));
}

HWY_ATTR uint32_t highway_extract_popcnt_32(V v_t) {
    const ScalableTag<uint32_t> d; 

    uint32_t t = 0; 

    for(int i = 0; i < Lanes(d); i += 8) {
        // unrolled for better performance, not portable for Systems with vector units smaller than 256-Bit
        t += __builtin_popcount(ExtractLane(v_t, i + 0));
        t += __builtin_popcount(ExtractLane(v_t, i + 1));
        t += __builtin_popcount(ExtractLane(v_t, i + 2));
        t += __builtin_popcount(ExtractLane(v_t, i + 3));
        t += __builtin_popcount(ExtractLane(v_t, i + 4));
        t += __builtin_popcount(ExtractLane(v_t, i + 5));
        t += __builtin_popcount(ExtractLane(v_t, i + 6));
        t += __builtin_popcount(ExtractLane(v_t, i + 7));
    }

    return t; 
}

HWY_ATTR uint32_t highway_extract_popcnt_64(V input) {
    const ScalableTag<uint64_t> d; 
    uint64_t t = 0; 
    auto vec = BitCast(d,input);

    for(int i = 0; i  < Lanes(d); i += 4) {
        t += __builtin_popcountl(ExtractLane(vec, i));
        t += __builtin_popcountl(ExtractLane(vec, i + 1));
        t += __builtin_popcountl(ExtractLane(vec, i + 2));
        t += __builtin_popcountl(ExtractLane(vec, i + 3));
    }

    return (uint32_t) t; 
}


HWY_ATTR uint32_t highway_store_popcnt(V v_t) {
    __attribute__((aligned(64))) uint32_t vec[Lanes(d)]; 
    Store(v_t, d, vec);
    uint32_t t = 0; 

    for(size_t i = 0; i < Lanes(d); i++) {
        t += __builtin_popcount(vec[i]);
    }

    return t; 
}

#ifndef SVE
uint32_t avx2_extract_popcnt_64(__m256i v_t){
    uint64_t t = 0;

    t += __builtin_popcountl(_mm256_extract_epi64(v_t, 0));
    t += __builtin_popcountl(_mm256_extract_epi64(v_t, 1));
    t += __builtin_popcountl(_mm256_extract_epi64(v_t, 2));
    t += __builtin_popcountl(_mm256_extract_epi64(v_t, 3));

    return (uint32_t) t;
}
#endif

#ifdef AVX512
uint32_t avx512_popcnt_reduce_32(__m512i v_t) {
    v_t = _mm512_popcnt_epi32(v_t);
    return (uint32_t) _mm512_reduce_add_epi32(v_t); 
}
#endif
