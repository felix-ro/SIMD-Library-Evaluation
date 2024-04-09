#ifndef SVE
#include <nsimd/nsimd-all.hpp>

#include "mandelbrotSettings.hpp"
#include "nsimdBaseMandelbrot.hpp"

void mandelbrot_nsimdBase(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image) {
    using namespace nsimd;

    vf32 test = set1(1.0f, f32());
    
    float xScale = (xEnd - xBegin) / width;
    float yScale = (yEnd - yBegin) / height;

    vf32 xScaleVec = set1(xScale, f32());
    vf32 xBeginVec = set1(xBegin, f32());
    vf32 bailoutVec = set1((float) BAILOUT, f32());

    vf32 c_real_increment = set1(1.0f, f32());

    for (size_t j = 0; j < height; j++) {
        vf32 c_imag = set1(yBegin + (j * yScale), f32());
        for (size_t i = 0; i < width; i += len(f32())) {
            vf32 c_real = iota(f32());
            c_real = add(c_real, c_real_increment, f32());
            c_real_increment = set1((float) i, f32());

            c_real = fma(c_real, xScaleVec, xBeginVec, f32());

            vf32 z_real = set1(0.0f, f32());
            vf32 z_imag = set1(0.0f, f32());

            int iteration = 0; 
            while(1) {
                iteration++;

                vf32 z_real_squared = mul(z_real, z_real, f32()); 
                vf32 z_imag_squared = mul(z_imag, z_imag, f32());
                vf32 temp = mul(z_real, z_imag, f32());

                z_real = add(sub(z_real_squared, z_imag_squared, f32()), c_real, f32());
                z_imag = add(add(temp, temp, f32()), c_imag, f32());

                /* masking of bailout values */
                vf32 norm = add(z_real_squared, z_imag_squared, f32());
                auto mask = lt(norm, bailoutVec, f32()); 

                if (iteration > MAX_ITERATIONS || !any(mask, f32())) {
                    vf32 oneVec = set1(1.0f, f32());
                    vf32 zeroVec = set1(0.0f, f32());

                    vf32 result = if_else1(mask, oneVec, zeroVec, f32());
                    storea(&image[(j * width) + i], result, f32());
                    break;
                }
            }
        }
    }
}
#endif
