#include <iostream>
#include <chrono>

#ifndef SVE
#include "hwy/aligned_allocator.h"
#include <hwy/highway.h>
#include <simdpp/simd.h>
#endif 	// SVE

#include "../mandelbrot/mandelbrot.hpp"
#include "../mandelbrot/nsimdMandelbrot.hpp"
#include "../mandelbrot/nsimdBaseMandelbrot.hpp"
#include "../mandelbrot/simdeMandelbrot.hpp"
#include "../utils/utils.hpp"


using std::chrono::high_resolution_clock;
using std::chrono::duration;

using namespace std; 

void runMandelbrotClassic(const size_t width, const size_t height) {
    float image[width * height];

    mandelbrot_autoVec(-1.5f, 0.5f, -1.0f, 1.0f, width, height, image); 

    char name[23] = "mandelbrot_classic.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}

#ifndef SVE
void runHighway(const size_t width, const size_t height) {
    hwy::AlignedFreeUniquePtr<float []> image = hwy::AllocateAligned<float>(width * height);

    mandelbrot_highway(-1.5f, 0.75f, -1.125f, 1.125f, width, height, &image[0]);

    char name[23] = "mandelbrot_highway.pbm";
    createBitmapImage(width, height, &image[0], name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}
#endif

#ifndef SVE
#ifndef NEON
void runVc(const size_t width, const size_t height) {
    float image[width * height];

    mandelbrot_vc(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[18] = "mandelbrot_VC.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}
#endif	// NEON
#endif 	// SVE

#ifndef SVE
#ifndef NEON
void runAVX2(const size_t width, const size_t height) {
    __attribute__((aligned(32))) float image[width * height];

    mandelbrot_avx2(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[20] = "mandelbrot_AVX2.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}
#endif	// NEON
#endif 	// SVE

#ifndef SVE
void runLibsimd(const size_t width, const size_t height) {
    simdpp::aligned_allocator<float, 32> allocator; 
    float * image = allocator.allocate(width * height); 

    mandelbrot_libsimdpp(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image);

    char name[25] = "mandelbrot_libsimdpp.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\tCOMPLETED" << std::endl;
}
#endif

void runPureSimd(const size_t width, const size_t height) {
    float image[width * height] = {0};
    
    mandelbrot_pure_simd(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[24] = "mandelbrot_pureSimd.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\tCOMPLETED" << std::endl;
}

#ifndef SVE
void runNSIMD(const size_t width, const size_t height) {
    __attribute__((aligned(64))) float image[width * height];

    mandelbrot_nsimd(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[21] = "mandelbrot_NSIMD.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}
#endif

#ifndef SVE
void runNSIMDBase(const size_t width, const size_t height) {
    __attribute__((aligned(64))) float image[width * height];

    mandelbrot_nsimdBase(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[26] = "mandelbrot_NSIMD_BASE.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\tCOMPLETED" << std::endl;
}
#endif

void runSIMDe(const size_t width, const size_t height) {
    __attribute__((aligned(64))) float image[width * height];

    mandelbrot_simde_avx2(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image); 

    char name[21] = "mandelbrot_SIMDe.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}

#ifdef NEON 
void runNEON(const size_t width, const size_t height) {
    float image[width * height] = {0};

    mandelbrot_neon(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image);

    for (size_t j = 0; j < height; j++) {
         for (size_t i = 0; i < width; i++) {
             std::cout << image[j * width + i];
         }
         std::cout << "\n";
    }

    char name[24] = "mandelbrot_neon.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\tCOMPLETED" << std::endl;
}
#endif


#ifdef SVE
void runSVE(const size_t width, const size_t height) {
    float image[width * height] = {0};

    mandelbrot_sve(-1.5f, 0.75f, -1.125f, 1.125f, width, height, image);

    //for (size_t j = 0; j < height; j++) {
    //     for (size_t i = 0; i < width; i++) {
    //         std::cout << image[j * width + i];
    //     }
    //     std::cout << "\n";
    //}

    char name[24] = "mandelbrot_sve.pbm";
    createBitmapImage(width, height, image, name);
    std::cout << name <<":\t\tCOMPLETED" << std::endl;
}
#endif	// SVE

int main() {
	std::cout << "Starting Test...\n" << std::endl; 

    	const size_t width = 1408;
    	const size_t height = 1408;
	    
    	runMandelbrotClassic(width, height);
	
	#ifndef SVE
	#ifndef NEON
    	runVc(width, height);
	#endif	// NEON
	#endif	// SVE
	
	#ifndef SVE
	runHighway(width, height);
	#endif

	#ifndef SVE
	#ifndef NEON
	runAVX2(width, height);
	#endif	// NEON
	#endif 	// SVE
    	
	#ifndef SVE
	runLibsimd(width, height);
	#endif // SVE

    runPureSimd(width, height);

    #ifndef SVE
    runNSIMD(width, height); 
	#endif

    #ifndef SVE
    runNSIMDBase(width, height); 
	#endif

    runSIMDe(width, height);

	#ifdef NEON
	runNEON(width, height);
	#endif

	#ifdef SVE
	runSVE(width, height);
	#endif	// SVE


    std::cout << "\nFinished Test!" << std::endl << "Check generated images!" << std::endl;  
    return 0;
}
