#pragma once

#include <SFML/Graphics.hpp>

#include "myrandom.h"
#include "const.h"

using namespace sf;

struct Planet {
	Vector2f pos, prevpos, vel, acc; 
	float size, mass;
	Color colour;
	CircleShape shape;

	Planet(); //Random planet generator
	Planet(float PosX, float PosY, float S, Color C, float Mass);
	void shapeSetup();
	void updatePosVel();
	void initialNudge(float dx, float dy); //Basically declaring initial velocity
};