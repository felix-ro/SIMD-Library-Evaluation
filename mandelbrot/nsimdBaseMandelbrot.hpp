#ifndef SVE

#ifndef nsimdBaseMandelbrot
#define nsimdBaseMandelbrot

void mandelbrot_nsimdBase(float xBegin, float xEnd, 
                      float yBegin, float yEnd,
                      size_t width, size_t height, float * image);
#endif	// nsimdBaseMandelbrot
#endif 	// SVE
