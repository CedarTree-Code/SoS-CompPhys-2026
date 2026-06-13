#include "myrandom.h"
#include <random>

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