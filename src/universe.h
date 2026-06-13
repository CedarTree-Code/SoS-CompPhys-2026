#pragma once

#include "planet.h"

using namespace sf;

class Universe {
	int N; //Number of planets 
	std::vector<Planet> planets; 
	float G = GRAVITATIONAL_CONST, dist, mindist, PEij;
	Vector2f dpos, force;
	double KE, PE;

	public:
	Universe(); //Makes a universe with 0 planets
	Universe(int n, bool random); //Makes a universe with n planets, if random==true then fills it with random planets
	Universe(int m, Planet* list, int n); //Takes an m element array of planets and makes a universe with n total planets

    void addPlanet(Planet p); //Adds planet p to the universe
    void addPlanets(int m, Planet* list); //Adds a list of m planets to the universe
	void show(RenderWindow& space); //Rendering
	void simulate(); //Computations

	double getKE();
	double getPE();
	double getTE();
};