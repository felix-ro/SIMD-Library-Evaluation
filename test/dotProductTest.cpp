#include <iostream>
#include <assert.h>
#include <numeric>

#include "../dotProduct/dotProduct.hpp"
#include "../dotProduct/dotProductHighway.hpp"
#include "../utils/utils.hpp"

void dot_product_unrolled_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_unrolled(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_unrolled \t\tPASSED" << std::endl; 
}

void dot_product_modified_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_modified(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_modified \t\tPASSED" << std::endl; 
}

void dot_product_openMP_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_openMP(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_openMP \t\tPASSED" << std::endl; 
}

void dot_product_AVX2_test(float * a, float *b, size_t length, float expected) {
        float result = dot_product_AVX2(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_AVX2 \t\tPASSED" << std::endl; 
}

void dot_product_AVX2_unrolled_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_AVX2_unrolled(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_AVX2_unrolled \tPASSED" << std::endl; 
}

void dot_product_libsimdpp_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_libsimdpp(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_libsimdpp \t\tPASSED" << std::endl; 
}

void dot_product_libsimdpp_unrolled_test(float * a, float *b, size_t length, float expected) {
    float result = dot_product_libsimdpp_unrolled(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_libsimdpp_unrolled \tPASSED" << std::endl; 
}

void dot_product_pure_simd_test(float * a, float * b, size_t length, float expected) {
    float result = dot_product_pure_simd(a, b, length); 
    assert(result == expected);
    std::cout << "dot_product_pure_simd \t\tPASSED" << std::endl; 
}

void dot_product_vc_test(float * a, float * b, size_t length, float expected) {
    float result = dot_product_vc(a, b, length);
    assert(result == expected);
    std::cout << "dot_product_vc \t\t\tPASSED" << std::endl;
}

void dot_product_vc_unrolled_test(float * a, float * b, size_t length, float expected) {
    float result = dot_product_vc_unrolled(a, b, length);
    assert(result == expected);
    std::cout << "dot_product_vc_unrolled \tPASSED" << std::endl;
}

#ifdef AVX512
void dot_product_avx512_test(float * a, float * b, size_t length, float expected) {
    float result = dot_product_avx512(a, b, length);
    assert(result == expected);
    std::cout << "dot_product_avx512 \t\tPASSED" << std::endl;
}

void dot_product_avx512_unrolled_test(float * a, float * b, size_t length, float expected) {
    float result = dot_product_avx512_unrolled(a, b, length);
    assert(result == expected);
    std::cout << "dot_product_avx512_unrolled \tPASSED" << std::endl;
}
#endif


int main () {
    size_t length = 64;
    __attribute__((aligned(64))) float a[length]; 
    __attribute__((aligned(64))) float b[length];

    std::iota(a, a + length, 1);
    std::iota(b, b + length, 1);


    float result = dot_product(a, b, length);
    std::cout << "Correct result = " << result << std::endl;

    dot_product_unrolled_test(a, b, length, result);
    dot_product_modified_test(a, b, length, result);
    dot_product_openMP_test(a, b, length, result);
    dot_product_AVX2_test(a, b, length, result);
    dot_product_AVX2_unrolled_test(a, b, length, result);
    dot_product_libsimdpp_test(a, b, length, result);
    dot_product_libsimdpp_unrolled_test(a, b, length, result);
    dot_product_pure_simd_test(a, b, length, result);
    dot_product_vc_test(a, b, length, result);
    dot_product_vc_unrolled_test(a, b, length, result);

#ifdef AVX512
    dot_product_avx512_test(a, b, length, result);
    dot_product_avx512_unrolled_test(a, b, length, result);
#endif
}

