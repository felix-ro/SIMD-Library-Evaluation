#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream> 
#include <cstring>

#include "utils.hpp"

using std::chrono::duration;
using namespace std;


void printIntegerArray(int * array, const int width, const int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            cout << array[j*height + i] << " ";
        }
        cout << "\n" << ""; 
    }
}


void printTimesArray(duration<double, std::milli> * times, int length) {
    for (int i = 0; i < length; i++) {
        cout << times[i].count() << "ms\n";
    }
}


duration<double, std::milli> getAverageTime(duration<double, std::milli> * times, int length) {
    duration<double, std::milli> average; 
    for (int i = 0; i < length; i++) {
        if (i == 0) 
            average = times[i]; 
        else 
            average += times[i];
    }

    return average/length;
}


void fillFloatArrayRandom(float * array, int length) {
    srand((unsigned int)time(NULL));
    float a = 5.0;
    for (int i = 0; i < length; i++) {
        array[i] = float(rand())/float((RAND_MAX)) * a;
    }
}

void createBitmapImage(size_t width, size_t height, float * image, char * name) {

    char path[64] = "test/images/";
    FILE * f = fopen(strcat(path, name), "w+");

    if (f){
        fprintf(f, "P1\n");
        fprintf(f, "%ld %ld\n", width, height);

        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                fprintf(f, " %d", (int) image[j * width + i]);
            }
            fprintf(f, "\n");
        }
    }
}