#ifndef simdeMandelbrot
#define simdeMandelbrot

void mandelbrot_simde_avx2(float xBegin, float xEnd, 
                     float yBegin, float yEnd,
                     size_t width, size_t height, float * image); 
#endif