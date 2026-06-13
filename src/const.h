#pragma once

#define H_SIZE 1920
#define V_SIZE 1080
#define WINDOW_SIZE {H_SIZE, V_SIZE}

#define GRAVITATIONAL_CONST 10000.f
#define MAX_FORCE 600000.f
#define MAX_NUDGE 2.5

#define MIN_SIZE 1
#define MAX_SIZE 100
#define MIN_MASS 100
#define MAX_MASS 3000

#define D_t (float)(1./144)  //~ 1./fps --- global variable

