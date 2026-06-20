#pragma once

#include <iostream>
#include <random>
#include <chrono>

#include "const.h"

float fRandom(float min, float max); //Returns random float b/w min & max
float iRandom(int min, int max); //Returns random int b/w min & max
void randomSeedGen(int n);

float deNormalize(float l);

struct Timer {
    std::string functionName;
    std::chrono::duration<double> timeTaken;
    std::chrono::time_point<std::chrono::steady_clock> start, end;

    Timer(std::string name);
    ~Timer();
};