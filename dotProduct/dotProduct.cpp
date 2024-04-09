#include <assert.h>
#include <immintrin.h>
#include <stdlib.h>
#include <Vc/Vc>
#include <simdpp/simd.h>
#include <pure_simd.hpp>


float dot_product(float * a, float * b, int length) {
    float sum = 0; 
    for (int i = 0; i < length; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

float dot_product_unrolled(float * a, float * b, int length) {
    float sum0 = 0; 
    float sum1 = 0;

    for (int i = 0; i < length; i += 2) {
        sum0 += a[i] * b[i];
        sum1 += a[i+1] * b[i+1];
    }

    return sum0 + sum1;
}

float dot_product_modified(float *a, float *b, size_t length) {
  float sum = 0;
  const float* limit = a + length;
  for ( ; a < limit; a++, b++) {
    sum += a[0] * b[0];
  }
  return sum;
}

// OpenMP implementation
float dot_product_openMP(float * a, float * b, int length) {
    float sum = 0;
    #pragma omp simd
    for (int i = 0; i < length; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// AVX2 implementation
float dot_product_AVX2(float *a, float *b, size_t n) {
    assert(n % 8 == 0);
    __m256 sum = _mm256_setzero_ps();
     size_t limit = n - 7;

     for (size_t i = 0; i < limit; i += 8) {
      __m256 av = _mm256_load_ps(a + i);
      __m256 bv = _mm256_load_ps(b + i);
      sum = _mm256_fmadd_ps(av, bv, sum);
    }

    float buffer[8];
    _mm256_storeu_ps(buffer, sum);
    return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7];
}

// AVX2 implementation
float dot_product_AVX2_unrolled(float *a, float *b, size_t length) {
    assert(length % 32 == 0);
    __m256 sum0 = _mm256_setzero_ps();
    __m256 sum1 = _mm256_setzero_ps();
    __m256 sum2 = _mm256_setzero_ps();
    __m256 sum3 = _mm256_setzero_ps();

    for (size_t i = 0; i < length; i += 32) {
        __m256 av0 = _mm256_load_ps(a + i);
        __m256 bv0 = _mm256_load_ps(b + i);
        sum0 = _mm256_fmadd_ps(av0, bv0, sum0);
        
        __m256 av1 = _mm256_load_ps(a + i + 8);
        __m256 bv1 = _mm256_load_ps(b + i + 8);
        sum1 = _mm256_fmadd_ps(av1, bv1, sum1);

        __m256 av2 = _mm256_load_ps(a + i + 16);
        __m256 bv2 = _mm256_load_ps(b + i + 16);
        sum2 = _mm256_fmadd_ps(av2, bv2, sum2);

        __m256 av3 = _mm256_load_ps(a + i + 24);
        __m256 bv3 = _mm256_load_ps(b + i + 24);
        sum3 = _mm256_fmadd_ps(av3, bv3, sum3);
    }

    sum0 = _mm256_add_ps(sum0, sum1);
    sum2 = _mm256_add_ps(sum2, sum3);
    sum0 = _mm256_add_ps(sum0, sum2);
    float buffer[8];
    _mm256_storeu_ps(buffer, sum0);
    return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
           buffer[6] + buffer[7];
}

// Vc implementation
float dot_product_vc(float * a, float * b, size_t length) {
    size_t N = Vc::float_v::Size;
    assert(length % N == 0);

    Vc::float_v av, bv, sum;
    for (size_t j = 0; j < length; j += N) {
        for (size_t i = 0; i < N; i++) {
            av[i] = a[i + j];
            bv[i] = b[i + j];
        }
        sum += av * bv;
    }

    return sum.sum();
}

// Vc implementation with loop unrolling.
float dot_product_vc_unrolled(float * a, float * b, size_t length) {
    size_t N = Vc::float_v::Size;
    assert(length % (N * 4) == 0);
    Vc::float_v av0, av1, av2, av3, bv0, bv1, bv2, bv3, sum0, sum1, sum2, sum3;
    for (size_t j = 0; j < length; j += N * 4) {
        for (size_t i = 0; i < N; i++) {
            av0[i] = a[i + j];
            bv0[i] = b[i + j];
            av1[i] = a[i + j + N];
            bv1[i] = b[i + j + N];
            av2[i] = a[i + j + (N * 2)];
            bv2[i] = b[i + j + (N * 2)];
            av3[i] = a[i + j + (N * 3)];
            bv3[i] = a[i + j + (N * 3)];
        }
        sum0 += av0 * bv0;
        sum1 += av1 * bv1;
        sum2 += av2 * bv2;
        sum3 += av3 * bv3;
    }

    float result0 = 0;
    float result1 = 0;
    result0 = sum0.sum() + sum1.sum();
    result1 = sum2.sum() + sum3.sum();

    return result0 + result1;
}

//libsimdpp implementation
float dot_product_libsimdpp(float * a, float * b, size_t length) {
    using namespace simdpp;
    const size_t N = SIMDPP_FAST_FLOAT32_SIZE; 

    assert(length % N == 0); 
    float32<N> sum = splat(0);

    for (size_t i = 0; i < length; i += N) {
        float32<N> av = load(a + i);
        float32<N> bv = load(b + i);
        sum = fmadd(av, bv, sum);
    }

    return reduce_add(sum); 
}

float dot_product_libsimdpp_unrolled(float * a, float * b, size_t length) {
    using namespace simdpp;
    const size_t N = SIMDPP_FAST_FLOAT32_SIZE; 

    assert(length % N * 4 == 0); 
    float32<N> sum0 = splat(0);
    float32<N> sum1 = splat(0);
    float32<N> sum2 = splat(0);
    float32<N> sum3 = splat(0);

    for (size_t i = 0; i < length; i += N * 4) {
        float32<N> av0 = load(a + i);
        float32<N> bv0 = load(b + i);
        sum0 = fmadd(av0, bv0, sum0);

        float32<N> av1 = load(a + i + N);
        float32<N> bv1 = load(b + i + N);
        sum1 = fmadd(av1, bv1, sum1);

        float32<N> av2 = load(a + i + (N * 2));
        float32<N> bv2 = load(b + i + (N * 2));
        sum2 = fmadd(av2, bv2, sum2);

        float32<N> av3 = load(a + i + (N * 3));
        float32<N> bv3 = load(b + i + (N * 3));
        sum3 = fmadd(av3, bv3, sum3);
    }

    sum0 = add(sum0, sum1); 
    sum2 = add(sum2, sum3);
    sum0 = add(sum0, sum2); 

    return reduce_add(sum0); 
}

float dot_product_pure_simd(float * a, float * b, size_t length) {
    using namespace pure_simd;

    const size_t VECTOR_SIZE = 8; 
    using TargetVec = vector<float, VECTOR_SIZE>;

    auto sum0 = scalar<TargetVec>(0.0f);

    for (size_t i = 0; i < length; i += VECTOR_SIZE) {
        auto av = load_from<TargetVec>(a + i);
        auto bv = load_from<TargetVec>(b + i);
        sum0 = (av * bv) + sum0; 
    }

    float result = sum<TargetVec>(sum0, 0.0f);
    return result;
}

#ifdef AVX512

float dot_product_avx512(float * a, float * b, size_t length) {
    assert(length % 16 == 0);
    __m512 sum = _mm512_setzero_ps();
    
    for (size_t i = 0; i < length; i += 16) {
	__m512 av = _mm512_load_ps(a + i);
	__m512 bv = _mm512_load_ps(b + i); 
	sum = _mm512_fmadd_ps(av, bv, sum);
    }

    return _mm512_reduce_add_ps(sum);
}

float dot_product_avx512_unrolled(float * a, float * b, size_t length) {
    assert(length % 64 == 0);
    __m512 sum0 = _mm512_setzero_ps();
    __m512 sum1 = _mm512_setzero_ps();
    __m512 sum2 = _mm512_setzero_ps();
    __m512 sum3 = _mm512_setzero_ps();

    for (size_t i = 0; i < length; i += 64) {
        __m512 av0 = _mm512_load_ps(a + i);
        __m512 bv0 = _mm512_load_ps(b + i);
        sum0 = _mm512_fmadd_ps(av0, bv0, sum0);

	__m512 av1 = _mm512_load_ps(a + i + 16);
        __m512 bv1 = _mm512_load_ps(b + i + 16);
        sum1 = _mm512_fmadd_ps(av1, bv1, sum1);

	__m512 av2 = _mm512_load_ps(a + i + 32);
        __m512 bv2 = _mm512_load_ps(b + i + 32);
        sum2 = _mm512_fmadd_ps(av2, bv2, sum2);

	__m512 av3 = _mm512_load_ps(a + i + 48);
        __m512 bv3 = _mm512_load_ps(b + i + 48);
        sum3 = _mm512_fmadd_ps(av3, bv3, sum3);
    }

    sum0 = _mm512_add_ps(sum0, sum1);
    sum2 = _mm512_add_ps(sum2, sum3);
    sum0 = _mm512_add_ps(sum0, sum2);

    return _mm512_reduce_add_ps(sum0);
}

#endif
























