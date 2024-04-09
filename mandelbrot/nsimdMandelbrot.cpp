#ifndef SVE
#include <nsimd/nsimd-all.hpp>

#include "mandelbrotSettings.hpp"
#include "nsimdMandelbrot.hpp"

void mandelbrot_nsimd(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    using namespace nsimd;
    typedef pack<float> floatv_t;
    typedef packl<float> maskv_t;

    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    floatv_t xScaleVec = set1<floatv_t>(xScale);
    floatv_t xBeginVec = set1<floatv_t>(xBegin);
    floatv_t bailoutVec = set1<floatv_t>((float) BAILOUT);

    floatv_t c_real_increment = set1<floatv_t>(1.0f);

    for (size_t j = 0; j < height; j++) {
            floatv_t c_imag = set1<floatv_t>(yBegin + (j * yScale));
        for (size_t i = 0; i < width; i += len<floatv_t>()) {
            floatv_t c_real = iota<floatv_t>();
            c_real = add(c_real, c_real_increment);
            c_real_increment = set1<floatv_t>((float) i);

            c_real = fma(c_real, xScaleVec, xBeginVec);

            floatv_t z_real = set1<floatv_t>(0.0f);
            floatv_t z_imag = set1<floatv_t>(0.0f);

            int iteration = 0; 
            while(1) {
                iteration++;

                floatv_t z_real_squared = mul(z_real, z_real); 
                floatv_t z_imag_squared = mul(z_imag, z_imag);
                floatv_t temp = mul(z_real, z_imag);

                z_real = add(sub(z_real_squared, z_imag_squared), c_real);
                z_imag = add(add(temp, temp), c_imag);

                /* masking of bailout values */
                floatv_t norm = add(z_real_squared, z_imag_squared);
                maskv_t mask = lt(norm, bailoutVec); 

                if (iteration > MAX_ITERATIONS || !any(mask)) {
                    floatv_t oneVec = set1<floatv_t>(1.0f);
                    floatv_t zeroVec = set1<floatv_t>(0.0f);

                    floatv_t result = if_else1(mask, oneVec, zeroVec);
                    storea(&image[(j * width) + i], result);
                    break;
                }
            }
        }
    }
}

#endif
