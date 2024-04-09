#include <assert.h>
#include <iostream>

#include "mandelbrotSettings.hpp"
#include "simdeMandelbrot.hpp"

#define SIMDE_ENABLE_NATIVE_ALIASES
#include "../../simde/simde/x86/avx2.h"

void mandelbrot_simde_avx2(float xBegin, float xEnd, 
                     float yBegin, float yEnd,
                     size_t width, size_t height, float * image) {
    assert((width * height) % 8 == 0);

    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;
    __m256 xScaleVec = _mm256_set1_ps(xScale);
    __m256 xBeginVec = _mm256_set1_ps(xBegin);
    __m256 bailoutVec = _mm256_set1_ps(BAILOUT);
    __m256 oneVec = _mm256_set1_ps(1);
    __m256 zeroVec = _mm256_setzero_ps();

    for (size_t j = 0; j < height; j++) {
        __m256 c_imag = _mm256_set1_ps (yBegin + (j * yScale));
        for (size_t i = 0; i < width; i += 8) {
            __m256 c_real = _mm256_set_ps(8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i, 2 + i, 1 + i);
            // c_real = _mm256_fmadd_ps(c_real, xScaleVec, xBeginVec);
	        c_real = _mm256_add_ps(_mm256_mul_ps(c_real, xScaleVec), xBeginVec);

            __m256 z_real = _mm256_setzero_ps();
            __m256 z_imag = _mm256_setzero_ps();

            int iteration = 0; 
            while(1) {
                iteration++; 

                __m256 z_real_squared = _mm256_mul_ps(z_real, z_real);
                __m256 z_imag_squared = _mm256_mul_ps(z_imag, z_imag); 
                __m256 temp = _mm256_mul_ps(z_real, z_imag); 

                z_real = _mm256_add_ps(_mm256_sub_ps(z_real_squared, z_imag_squared), c_real);
                z_imag = _mm256_add_ps(_mm256_add_ps(temp, temp), c_imag);

                __m256 norm = _mm256_add_ps(z_real_squared, z_imag_squared);

                __m256 mask = _mm256_cmp_ps(norm, bailoutVec, _CMP_LT_OQ);

                if (_mm256_movemask_ps(mask) == 0 || iteration > MAX_ITERATIONS) {
                    __m256 result = _mm256_blendv_ps(zeroVec, oneVec, mask);
                    _mm256_store_ps(&image[(j * width) + i], result);
                    break;
                }
            }
        }
    }
}
