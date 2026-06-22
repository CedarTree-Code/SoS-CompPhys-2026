#include "misc.h"

std::random_device rd;
std::mt19937 gen(rd());

float fRandom(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gen);
}

float iRandom(int min, int max) {
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}

float deNormalize(float l) {
    return l*N_SIZE;
}

void randomSeedGen(int n) {
    std::random_device rd;
    for(int i=0; i<n; i++)
        std::cout << rd() << std::endl;
}

Timer::Timer(std::string name) {
    functionName = name;
    start = std::chrono::steady_clock::now();
}

Timer::~Timer() {
    end = std::chrono::steady_clock::now();
    timeTaken = end - start;
    double ms = timeTaken.count()*1000.0;
    std::cout << "Time taken by " << functionName << " is " << ms << "ms" << std::endl;
}