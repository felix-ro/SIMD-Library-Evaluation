#ifndef MandelbrotScalar
#define MandelbrotScalar

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
void mandelbrot_scalar(float realBeginning, float realEnd, 
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
void mandelbrot_scalar_complexClass(float realBeginning, float realEnd, 
                      float imagBeginning, float imagEnd,
                      int width, int height, int * image);

#endif