#ifndef dotProduct
#define dotProduct 

#include <stdlib.h>


/**
 * Calculates the dot product of two vectors.
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param length
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product(float * a, float * b, int length);


/**
 * Calculates the dot product of two vectors using loop unrolling for independent sum accumulators.
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param length
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_unrolled(float * a, float * b, int length);


/**
 * Calculates the dot product of two vectors. The iteration is different compared to dot_product()
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param length
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_modified(float *a, float *b, size_t length);


/**
 * Calculates the dot product, using openMP simd.
 *
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param length
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_openMP(float * a, float * b, int length);


/**
 * Calculates the dot product of two vectors using AVX2. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_AVX2(float *a, float *b, size_t n);


/**
 * Calculates the dot product of two vectors using AVX2 and loop unrolling. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_AVX2_unrolled(float *a, float *b, size_t length);


/**
 * Calculates the dot product of two vectors using Vc. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_vc(float * a, float * b, size_t length);


/**
 * Calculates the dot product of two vectors using Vc with loop unrolling. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_vc_unrolled(float * a, float * b, size_t length);


/**
 * Calculates the dot product of two vectors using libsimdpp. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_libsimdpp(float * a, float * b, size_t length); 


/**
 * Calculates the dot product of two vectors using libsimdpp with loop unrolling. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_libsimdpp_unrolled(float * a, float * b, size_t length);


/**
 * Calculates the dot product of two vectors using pure_simd. 
 * 
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 * 
 * @return The dot product 
*/
float dot_product_pure_simd(float * a, float * b, size_t length);


#ifdef AVX512
/**
 * Calculates the dot product of two vectors using avx512.
 *
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 *
 * @return The dot product
*/
float dot_product_avx512(float * a, float * b, size_t length);


/**
 * Calculates the dot product of two vectors using avx512 with loop unrolling.
 *
 * @param a
 *          The first vector (float array)
 * @param b
 *          The second vector (float array)
 * @param n
 *          The length of the array
 *
 * @return The dot product
*/
float dot_product_avx512_unrolled(float * a, float * b, size_t length);
#endif	// AVX512

#endif  // dotProduct
