#include <iostream>
#include <assert.h>
#include <pure_simd.hpp>
#include <hwy/highway.h>
#include <hwy/print-inl.h> 

#include "mandelbrotSettings.hpp"
#include "mandelbrot.hpp"
#include "../utils/vecComplex.hpp"

#if !defined(NEON) && !defined(SVE)
#include <Vc/Vc>
#include <immintrin.h>
#endif  // NEON and SVE

#ifndef SVE
#include <simdpp/simd.h>
#endif	// SVE

#ifdef NEON
#include <arm_neon.h>
#endif 	// NEON include

#ifdef SVE
#include <arm_sve.h>
#endif 	// SVE include

void mandelbrot_autoVec(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      int width, int height, float * image) {
    
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    float c_real, c_imag, z_real, z_imag, temp, z_real_squared, z_imag_squared; 

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            c_real = xBegin + i * xScale;
            c_imag = yBegin + j * yScale;
            z_real = 0.0;
            z_imag = 0.0;
            
            int iteration = 0; 
            while (1) {
                iteration++;
                temp = z_real * z_imag;
                z_real_squared = z_real * z_real;
                z_imag_squared = z_imag * z_imag;
                z_real = z_real_squared - z_imag_squared + c_real;
                z_imag = temp + temp + c_imag;

                if (z_imag_squared + z_real_squared > BAILOUT) {
                    * image++ = 0.0f;
                    break;
                }
                        
                if (iteration > MAX_ITERATIONS) {
                    * image++ = 1.0f; 
                    break;
                }
            }
        }
    }
}


void mandelbrot_autoVec_complexClass(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      int width, int height, int * image) {
    typedef VecComplex<float> Z; 
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    float c_real, c_imag;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            c_real = xBegin + i * xScale;
            c_imag = yBegin + j * yScale;

            Z z(0.0f + c_real, 0.0f + c_imag);

            int iteration = 0; 
            while (1) {
                iteration++;
                z = z.mandelIterate(c_real, c_imag);

                if (z.norm() > BAILOUT) {
                    * image++ = 0;
                    break;
                }
                        
                if (iteration > MAX_ITERATIONS) {
                    * image++ = 1; 
                    break;
                }
            }
        }
    }
}

void mandelbrot_openMP(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      int width, int height, float * image) {
    
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    float c_real, c_imag, z_real, z_imag, temp, z_real_squared, z_imag_squared; 

    for (int j = 0; j < height; j++) {
        #pragma omp simd aligned(image: 64) simdlen(8)
        for (int i = 0; i < width; i++) {

            c_real = xBegin + i * xScale;
            c_imag = yBegin + j * yScale;
            z_real = 0.0;
            z_imag = 0.0;
            
            int iteration = 0; 
            while (1) {
                iteration++;
                temp = z_real * z_imag;
                z_real_squared = z_real * z_real;
                z_imag_squared = z_imag * z_imag;
                z_real = z_real_squared - z_imag_squared + c_real;
                z_imag = temp + temp + c_imag;

                if (z_imag_squared + z_real_squared > BAILOUT) {
                    * image++ = 0.0f;
                    break;
                }
                        
                if (iteration > MAX_ITERATIONS) {
                    * image++ = 1.0f; 
                    break;
                }
            }
        }
    }
}

#ifndef NEON
#ifndef SVE
void mandelbrot_avx2(float xBegin, float xEnd, 
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
            c_real = _mm256_fmadd_ps(c_real, xScaleVec, xBeginVec);

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
#endif
#endif

HWY_BEFORE_NAMESPACE();
HWY_ATTR void mandelbrot_highway(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height,
                      float* const HWY_RESTRICT image) {
    using namespace hwy; 
    using namespace HWY_NAMESPACE;

    const ScalableTag<float> d;
    const size_t N = Lanes(d);
    using V = decltype(Zero(d));
    
    assert((width * height) % N == 0); // Ensure that vector lanes fit

    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    auto xScaleVec = Set(d, xScale);
    auto xBeginVec = Set(d, xBegin); 
    auto bailoutVec = Set(d, BAILOUT);

    for (size_t j = 0; j < height; j++) {
            auto c_imag = Set(d, yBegin + (j * yScale));
        for (size_t i = 0; i < width; i += N) {
            auto c_real = Iota(d, i);
            c_real = MulAdd(c_real, xScaleVec, xBeginVec);

            V z_real = Zero(d);
            V z_imag = Zero(d);

            int iteration = 0; 
            while(1) {
                iteration++;

                auto z_real_squared = Mul(z_real, z_real); 
                auto z_imag_squared = Mul(z_imag, z_imag);
                auto temp = Mul(z_real, z_imag);
                
                z_real = Add(Sub(z_real_squared, z_imag_squared), c_real);
                z_imag = Add(Add(temp, temp), c_imag); 

                /* masking of bailout values */
                auto norm = Add(z_real_squared, z_imag_squared);
                auto mask = Lt(norm, bailoutVec);

                if (iteration > MAX_ITERATIONS || AllFalse(d, mask)) {
                    auto oneVec = Set(d, 1);
                    auto result = IfThenElseZero(mask, oneVec);
 
                    Store(result, d, &image[(j * width) + i]);
                    break;
                }
            }
        }
    }
}
HWY_AFTER_NAMESPACE();


#ifndef NEON
#ifndef SVE
void mandelbrot_vc(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    using namespace Vc; 
    assert((width * height) % float_v::Size == 0); // Ensure that vector lanes fit
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    for (size_t j = 0; j < height; j++) {
        float_v c_imag = yBegin + j * yScale;
        uint_v x = uint_v::IndexesFromZero();
        for (size_t i = 0; i < width; i += float_v::Size) {
            float_v c_real = xBegin + simd_cast<float_v>(x) * xScale;
            x += (int) float_v::Size;

            float_v z_real = float_v::Zero();
            float_v z_imag = float_v::Zero();

            int iteration = 0; 
            while (1) {
                iteration++; 

                float_v z_real_squared = z_real * z_real;
                float_v z_imag_squared = z_imag * z_imag; 
                float_v temp = z_real * z_imag;

                z_real = (z_real_squared - z_imag_squared) + c_real;
                z_imag = temp + temp + c_imag;

                float_v norm = z_real_squared + z_imag_squared;

                float_m mask = norm < BAILOUT;
                if (mask.isEmpty() || iteration > MAX_ITERATIONS) {
                    float_v result = float_v::Zero();
                    ++result(mask);
                    result.store(&image[(j * width) + i], Vc::Unaligned);
                    break; 
                }
            }
        }
    }
}
#endif
#endif

#ifndef SVE
void mandelbrot_libsimdpp(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    using namespace simdpp;
    const size_t N = SIMDPP_FAST_FLOAT32_SIZE; 
    assert((width * height) % N == 0); 
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    float32<N> xScaleVec = splat(xScale);
    float32<N> xBeginVec = splat(xBegin);
    float32<N> bailoutVec = splat(BAILOUT);
    float32<N> zeroVec = splat(0);
    float32<N> oneVec = splat(1);
 
    for (size_t j = 0; j < height; j++) {
        float32<N> c_imag = splat(yBegin + (j * yScale)); 
        
        for (size_t i = 0; i < width; i += N) {
            float32<N> c_real;
            if (N == 8) {
                c_real = make_float(1+i, 2+i, 3+i, 4+i, 5+i, 6+i, 7+i, 8+i);
            } else if (N == 16) {
                c_real = make_float(1+i, 2+i, 3+i, 4+i, 5+i, 6+i, 7+i, 8+i, 9+i, 10+i, 11+i, 12+i, 13+i, 14+i, 15+i, 16+i);
            }
     

            c_real = fmadd(c_real, xScaleVec, xBeginVec);

            float32<N> z_real = splat(0);
            float32<N> z_imag = splat(0); 

            int iteration = 0; 
            while(1) {
                iteration++; 

                float32<N> z_real_squared = mul(z_real, z_real);
                float32<N> z_imag_squared = mul(z_imag, z_imag);
                float32<N> temp = mul(z_real, z_imag);

                z_real = add(sub(z_real_squared, z_imag_squared), c_real);
                z_imag = add(add(temp, temp), c_imag); 

                float32<N> norm = add(z_real_squared, z_imag_squared);
                mask_float32<N> mask = cmp_lt(norm, bailoutVec);
                float32<N> result = blend(oneVec, zeroVec, mask);
                
                if (!test_bits_any(result) || iteration > MAX_ITERATIONS) {
                    store(image + i + j*width, result);
                    break; 
                }
            }
        }
    }
}
#endif

void mandelbrot_pure_simd(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    using namespace pure_simd;
    const size_t VECTOR_SIZE = 8;   // Defines how many times the code will be unrolled
    using TargetVec = vector<float, VECTOR_SIZE>;

    assert((width * height) % VECTOR_SIZE == 0); 
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    auto xScaleVec = scalar<TargetVec>(xScale);
    auto xBeginVec = scalar<TargetVec>(xBegin);

    for (size_t j = 0; j < height; j++) {
        auto c_imag = scalar<TargetVec>(yBegin + (j * yScale));

        for (size_t i = 0; i < width; i += VECTOR_SIZE) {
            auto c_real = iota<TargetVec, size_t>(i, 1.0f);
            c_real = (c_real * xScaleVec) + xBeginVec;

            auto z_real = scalar<TargetVec>(0.0f);
            auto z_imag = scalar<TargetVec>(0.0f);

            int iteration = 0;
            while(1) {
                iteration++; 

                auto z_real_squared = z_real * z_real; 
                auto z_imag_squared = z_imag * z_imag;
                auto temp = z_real * z_imag;

                z_real = (z_real_squared - z_imag_squared) + c_real;
                z_imag = (temp + temp) - c_imag; 

                auto norm = z_real_squared + z_imag_squared;
                auto mask = norm < scalar<TargetVec>((float) BAILOUT);

                bool allFalse = true;
                for (size_t x = 0; x < VECTOR_SIZE; x++) {
                    if (mask[x] == 1) {
                        allFalse = false;
                        break; 
                    }
                }

                if (allFalse || iteration > MAX_ITERATIONS) {
                    for (size_t x = 0; x < VECTOR_SIZE; x++) {
                        if (mask[x] == 0) {
                            image[(j * width) + i + x] = 0;
                        } else {
                            image[(j * width) + i + x] = 1;
                        }
                    }
                    break;
                }
            }
        }
    }
}

#ifndef NEON
#ifndef SVE
#ifdef AVX512
void mandelbrot_avx512(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    assert((width * height) % 16 == 0);
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;
    
    
    __m512 xScaleVec  = _mm512_set1_ps(xScale);
    __m512 xBeginVec = _mm512_set1_ps(xBegin);
    __m512 bailoutVec = _mm512_set1_ps(BAILOUT);
    __m512 oneVec = _mm512_set1_ps(1);
    __m512 zeroVec = _mm512_setzero_ps();

    for (size_t j = 0; j < height; j++) {
    	__m512 c_imag = _mm512_set1_ps(yBegin + (j * yScale));
	
        for (size_t i = 0; i < width; i += 16) {
            __m512 c_real = _mm512_set_ps(16+i, 15+i, 14+i, 13+i, 12+i, 11+i, 10+i, 9+i,
                8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i, 2 + i, 1 + i);
            c_real = _mm512_fmadd_ps(c_real, xScaleVec, xBeginVec);
        
            __m512 z_real = _mm512_setzero_ps();
            __m512 z_imag = _mm512_setzero_ps();

            int iteration = 0;
            while (1) {
                iteration++;

                __m512 z_real_squared = _mm512_mul_ps(z_real, z_real);
                __m512 z_imag_squared = _mm512_mul_ps(z_imag, z_imag); 
                __m512 temp = _mm512_mul_ps(z_real, z_imag);

                z_real = _mm512_add_ps(_mm512_sub_ps(z_real_squared, z_imag_squared), c_real);
                z_imag = _mm512_add_ps(_mm512_add_ps(temp, temp), c_imag);

                __m512 norm = _mm512_add_ps(z_real_squared, z_imag_squared);

                __mmask16 mask = _mm512_cmp_ps_mask(norm, bailoutVec, _CMP_LT_OQ);

                if ((int) mask  == 0 || iteration > MAX_ITERATIONS) {
                    _mm512_store_ps(&image[(j * width) + i], _mm512_mask_blend_ps(mask, oneVec, zeroVec));
                break;
                }
            }
        }
    }  
}
#endif // AVX512
#endif // SVE
#endif // NEON


#ifdef NEON
void mandelbrot_neon(float xBegin, float xEnd,
                     float yBegin, float yEnd,
                     size_t width, size_t height, float * image) {
    const size_t LANE_SIZE = 4;
    assert((width * height) % LANE_SIZE == 0);
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    float32x4_t xScaleVec = vdupq_n_f32(xScale);
    float32x4_t xBeginVec = vdupq_n_f32(xBegin);
    float32x4_t bailoutVec = vdupq_n_f32(BAILOUT);
    uint32x4_t oneVec = vdupq_n_u32(1);

    for (size_t j = 0; j < height; j++) {
	    float32x4_t c_imag = vdupq_n_f32(yBegin + (j* yScale));
        for (size_t i = 0; i < width; i += LANE_SIZE) {
            const float c_real_arr[LANE_SIZE] = {(float)i+1, (float)i+2, (float)i+3, (float)i+4};
            float32x4_t c_real = vld1q_f32(c_real_arr);
	        c_real = vaddq_f32(vmulq_f32(c_real, xScaleVec), xBeginVec);

            float32x4_t z_real = vdupq_n_f32(0);
            float32x4_t z_imag = vdupq_n_f32(0);

            int iteration = 0; 
            while (1) {
                iteration++;

                float32x4_t z_real_squared = vmulq_f32(z_real, z_real);
                float32x4_t z_imag_squared = vmulq_f32(z_imag, z_imag);
                float32x4_t temp = vmulq_f32(z_real, z_imag);

                z_real = vaddq_f32(vsubq_f32(z_real_squared, z_imag_squared), c_real);
                z_imag = vaddq_f32(vaddq_f32(temp, temp), c_imag);

                float32x4_t norm = vaddq_f32(z_real_squared, z_imag_squared);
		        uint32x4_t mask = vcltq_f32(norm, bailoutVec);

                if (iteration > MAX_ITERATIONS) {
                    float32x4_t result = vcvtq_f32_u32(vandq_u32(mask, oneVec));
                    vst1q_f32(&image[j * width + i], result);
                    break;
                }

                if (vaddvq_u32(vandq_u32(mask, oneVec)) == 0) {
                    float32x4_t result = vcvtq_f32_u32(vandq_u32(mask, oneVec));
                    vst1q_f32(&image[j * width + i], result);
                    break;
                }
            }
        }
    }
}
#endif


#ifdef SVE
void mandelbrot_sve(float xBegin, float xEnd,
                     float yBegin, float yEnd,
                     size_t width, size_t height, float * image) {
	const uint64_t N = svcntw();
    assert((width * height) % N == 0);
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

	svbool_t allTrue = svdup_b32(true);
	svfloat32_t xScaleVec = svdup_f32(xScale);
	svfloat32_t xBeginVec = svdup_f32(xBegin);
	svfloat32_t bailoutVec = svdup_f32(BAILOUT);
	svfloat32_t oneVec = svdup_f32(1);
	svfloat32_t zeroVec = svdup_f32(0);

	for (size_t j = 0; j < height; j++) {
		svfloat32_t c_imag = svdup_f32(yBegin + (j* yScale));	
		for (size_t i = 0; i < width; i += N) {
			svfloat32_t c_real = svcvt_f32_s32_x(svptrue_b32(), svindex_s32((int32_t) i+1, (int32_t) 1));
			c_real = svadd_f32_x(allTrue, svmul_f32_m(allTrue, c_real, xScaleVec), xBeginVec); 

			svfloat32_t z_imag = svdup_f32(0);
			svfloat32_t z_real = svdup_f32(0);
			
			int iteration = 0; 
			while(1) {
				iteration++; 
				
				svfloat32_t z_real_squared = svmul_f32_m(allTrue, z_real, z_real);
				svfloat32_t z_imag_squared = svmul_f32_m(allTrue, z_imag, z_imag); 
				svfloat32_t temp = svmul_f32_m(allTrue, z_real, z_imag);  

				z_real = svadd_f32_x(allTrue, svsub_f32_m(allTrue, z_real_squared, z_imag_squared), c_real);
				z_imag = svadd_f32_x(allTrue, svadd_f32_x(allTrue, temp, temp), c_imag);

				svfloat32_t norm =  svadd_f32_x(allTrue, z_real_squared, z_imag_squared);
				svbool_t mask = svcmplt_f32(allTrue, norm, bailoutVec);

				if (iteration > MAX_ITERATIONS) {
					svst1_f32(mask, &image[i + (j*width)], oneVec);
					svbool_t negMask = svbic_b_z(allTrue, allTrue, mask); 
					svst1_f32(negMask, &image[i + (j*width)], zeroVec); 	
					break;
				}

				if (svcntp_b32(allTrue, mask) == 0) {
					svst1_f32(allTrue, &image[i + (j*width)], zeroVec); 
					break; 
				}
			}
		}
	}
}
#endif
