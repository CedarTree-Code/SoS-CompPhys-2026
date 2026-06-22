#pragma once

#define H_SIZE 1920
#define V_SIZE 1080
#define WINDOW_SIZE {H_SIZE, V_SIZE}
#define N_SIZE std::max(H_SIZE, V_SIZE)
#define SCREEN_STATE State::Fullscreen

#define SIMULATION_TYPE 2 //1 -> Normal, 2 -> Barnes-Hut
#define Z 0.6 //s/d
#define NO_OF_PLANETS 2500 //1 -> 20000 barely inches along, 2 -> 80000 at Z=0.75 does similar

//for testing
// #define TEST_ITER 24
// #define Z1 0.1 
// #define Z2 0.5 
// #define Z3 0.7 

#define GRAVITATIONAL_CONST 1e-5f
#define MAX_FORCE 1000.f
#define MAX_NUDGE 1e-8f

#define MIN_SIZE 5
#define MAX_SIZE 10
#define MIN_MASS 100
#define MAX_MASS 1000

#define D_t (float)(1./144)  //~ 1./fps --- global variable

#define MAX_PER_NODE_COUNT (int)1
#define BOX_THICKNESS .75f


