#ifndef mandelbrot 
#define mandelbrot

#include <hwy/highway.h>

/**
 * Calculates the image of the mandelbrot set with the given dimensions. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_autoVec(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      int width, int height, float * image);


/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  using light weight functions for complex numbers. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_autoVec_complexClass(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      int width, int height, int * image);


/**
 * Calculates the image of the mandelbrot set with the given dimensions using openMP pragmas to support the auto vectorization. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_openMP(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      int width, int height, float * image);

#ifndef NEON
#ifndef SVE
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using intrinsics. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_avx2(float realBeginning, float realEnd, 
                     float imagBeginning, float imagEnd,
                     size_t width, size_t height, float * image);
#endif	// SVE
#endif	// NEON


/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using the Highway library. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
HWY_ATTR void mandelbrot_highway(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height,
                      float* const HWY_RESTRICT image);


#ifndef NEON
#ifndef SVE
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using the Vc library. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_vc(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#endif	// SVE
#endif	// NEON


#ifndef SVE
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using the libsimdpp library. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_libsimdpp(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#endif


/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using the pure simd library. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_pure_simd(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#ifndef NEON
#ifndef SVE
#ifdef AVX512
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using AVX512. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_avx512(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#endif  // AVX512
#endif  // SVE
#endif 	// NEON


#ifdef NEON
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using neon extension. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_neon(float realBeginning, float realEnd,
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#endif 	// NEON Implementation

#ifdef SVE 
/**
 * Calculates the image of the mandelbrot set with the given dimensions, 
 *  employing vectorization by using sve intrinsics. 
 * 
 * @param realBeginning
 *          The x value which sets the left boarder of the image
 * @param realEnd
 *          The x value which sets the right boarder of the image
 * @param imagBeginning
 *          The y value which sets the top boarder of the image  
 * @param imagEnd
 *          The y value which sets the lower boarder of the image
 * @param width 
 *          The width of the image
 * @param heigth
 *          The height of the image 
 * @param maxIterations
 *          The maximum number of iterations per number tested
 * @param image
 *          The immage array
 * 
*/
void mandelbrot_sve(float realBeginning, float realEnd,
                      float imagBeginning, float imagEnd,
                      size_t width, size_t height, float * image);
#endif	// SVE
#endif  // mandelbrot
