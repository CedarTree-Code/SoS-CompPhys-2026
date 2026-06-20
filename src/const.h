#pragma once

#define H_SIZE 1920
#define V_SIZE 1080
#define WINDOW_SIZE {H_SIZE, V_SIZE}
#define N_SIZE std::max(H_SIZE, V_SIZE)
#define SCREEN_STATE State::Fullscreen

#define SIMULATION_TYPE 2 //1 -> Normal, 2 -> Barnes-Hut
#define NO_OF_PLANETS 10 //1-> 20000 barely inches along, 2-> 80000 at Z=0.75 does same
#define SEED 1548649472

#define TEST_ITER 24

#define GRAVITATIONAL_CONST 1e-5f
#define MAX_FORCE 1000.f
#define MAX_NUDGE 1e-8f

#define MIN_SIZE 3
#define MAX_SIZE 3
#define MIN_MASS 100
#define MAX_MASS 1000

#define D_t (float)(1./144)  //~ 1./fps --- global variable

#define MAX_PER_NODE_COUNT (int)1
#define BOX_THICKNESS .75f
#define Z 0.1 //s/d

