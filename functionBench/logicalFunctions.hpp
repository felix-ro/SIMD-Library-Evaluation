#include <hwy/highway.h>
#include <immintrin.h>

using namespace hwy;
using namespace HWY_NAMESPACE;

const ScalableTag<uint32_t> d;
using V = decltype(Zero(d));

__m256i logicalAndTestAVX(__m256i a, __m256i b);
V logicalAndTestHighway(V a, V b); 

__m256i logicalOrTestAVX(__m256i a, __m256i b);
V logicalOrTestHighway(V a, V b);

__m256i logicalXorTestAVX(__m256i a, __m256i b);
V logicalXorTestHighway(V a, V b);

__m256i setFunctionAVX(uint32_t start);
V setFunctionHighway(uint32_t start);

V maskedLoadHighway(uint32_t * arr);