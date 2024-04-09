#ifndef utils
#define utils

#include <chrono> 
#include <iostream>

using std::chrono::duration;

/**
 * Prints an integer array of the specified width and heigth to the console.
 * 
 * @param array 
 *          The array to print
 * @param width
 *          The width of the array
 * @param height
 *          The height of the array
*/
void printIntegerArray(int * array, int width, int height);

/**
 * Prints an array filled with durations in milliseconds.
 * 
 * @param times
 *          The array to print
 * @param length
 *          The length of the array
*/
void printTimesArray(duration<double, std::milli> * times, int length);


/**
 * Calculates the average time.
 * 
 * @param times 
 *          The times
 * @param length
 *          The number of times
 * 
 * @return  The average time 
*/
duration<double, std::milli> getAverageTime(duration<double, std::milli> * times, int length);
#endif


/**
 * Fills an array with random floating point numbers.
 * 
 * @param array
 *          The array to fill
 * @param length
 *          The length of the array
*/
void fillFloatArrayRandom(float * array, int length);

/**
 * Creates a Bitmap image of a float array. 
 *
 * @param width
 *          The width of the image
 * @param height
 *          The height of the image
 * @param image
 *          The array for the image
 * @param name
 *          The file name with extension (.pbm)
*/
void createBitmapImage(size_t width, size_t height, float * image,  char * name);


template <typename T, typename F> void fillFloatArrayRandomTemp(T &array){
    srand((unsigned int)time(NULL));
    float a = 5.0;
    for (size_t i = 0; i < array.size(); i++) {
        array[i] = F(float(rand())/float((RAND_MAX)) * a);
    }
}

template <typename T> void printInteger(T * array, const size_t width, const size_t height) {
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            std::cout << array[j*height + i];
        }
        std::cout << "\n"; 
    }
}