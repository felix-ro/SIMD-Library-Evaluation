#include "logicalFunctions.hpp"

__m256i logicalAndTestAVX(__m256i a, __m256i b) {
    return _mm256_and_si256(a, b);
}

V logicalAndTestHighway(V a, V b) {
    return And(a, b);
}

__m256i logicalOrTestAVX(__m256i a, __m256i b) {
    return _mm256_or_si256(a, b);
}

V logicalOrTestHighway(V a, V b) {
    return Or(a, b);
}

__m256i logicalXorTestAVX(__m256i a, __m256i b) {
    return _mm256_xor_si256(a, b);
}

V logicalXorTestHighway(V a, V b) {
    return Xor(a, b);
}

__m256i setFunctionAVX(uint32_t start) {
    return _mm256_set_epi32(start+7, start+6, start+5, start+4, start+3, start+2, start+1, start+0);
}

V setFunctionHighway(uint32_t start) {
    return Add(Set(d, start), Iota(d, 0));
}

V maskedLoadHighway(uint32_t * arr) {
    auto mask = FirstN(d, 4);
    return MaskedLoad(mask, d, arr); 
}
