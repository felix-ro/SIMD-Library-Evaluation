#ifndef dotProductHighway
#define dotProductHighway

#include <hwy/highway.h>

/**
 * Calculates the dot product of two vectors using google highway.
 * 
 * @param pa 
 *          The first vector
 * @param pa 
 *          The second vector
 * @param numItems
 *          The number of items in each vector
 *
 * @return The dot product
 */
HWY_ATTR float highway_dot_product(const float* const HWY_RESTRICT pa, 
                          const float* const HWY_RESTRICT pb, size_t numItems);


/**
 * Calculates the dot product of two vectors using google highway and loop unrolling.
 * 
 * @param pa 
 *          The first vector
 * @param pa 
 *          The second vector
 * @param numItems
 *          The number of items in each vector
 *
 * @return The dot product
 */
float highway_dot_product_unrolled(const float* const HWY_RESTRICT pa, 
                          const float* const HWY_RESTRICT pb, size_t numItems);

#endif