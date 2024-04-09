#include "mandelbrotSettings.hpp"
#include "mandelbrotScalar.hpp"
#include "../utils/vecComplex.hpp"


void mandelbrot_scalar(float xBegin, float xEnd, 
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

void mandelbrot_scalar_complexClass(float xBegin, float xEnd, 
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
