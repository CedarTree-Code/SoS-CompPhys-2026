#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <matplot/matplot.h>
using namespace sf;

std::random_device rd;
std::mt19937 gen(rd());

#define H_SIZE 1600
#define V_SIZE 900
#define WINDOW_SIZE {H_SIZE, V_SIZE}

#define MAX_NUDGE 2.5
#define FORCE_MAX 600000.f

const float dt = 1./144; // ~ 1./fps --- global variable

float fRandom(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gen);
}

float iRandom(int min, int max) {
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}

struct Planet {

	Vector2f pos, prevpos, vel, acc; 
	float size, mass;
	Color colour;
	CircleShape shape;

	Planet() {
		//Random planet
		pos.x = fRandom(0, H_SIZE);
		pos.y = fRandom(0, V_SIZE);
		prevpos.x = pos.x + fRandom(-MAX_NUDGE, MAX_NUDGE);
		prevpos.y = pos.y + fRandom(-MAX_NUDGE, MAX_NUDGE);
		acc.x = acc.y = 0;
		size = fRandom(10, 150);
		mass = fRandom(100, 3000); 

		colour = Color(iRandom(0, 255), iRandom(0, 255), iRandom(0, 255)); 
		shapeSetup();
	}

	Planet(float PosX, float PosY, float S, Color C, float Mass) {
		pos.x = PosX; pos.y = PosY; //initial position
		acc.x = acc.y = 0; //initial acceleration
		mass = Mass;
		size = S; colour = C;
		prevpos = pos;
		shapeSetup(); //setup Planet shape for display
	}


	void shapeSetup() {
		shape.setRadius(size);
		shape.setOrigin({size, size});
		shape.setFillColor(colour);
	}

	void updatePosVel() {
		//Verlet integration//
		Vector2f temp = pos;
		pos = 2.f*pos - prevpos + acc*dt*dt; //O(t^4) error
		vel = (pos - prevpos)/(2*dt); //O(t^2) error
		prevpos = temp;
	}

	void initialNudge(float dx, float dy) {
		prevpos = pos + Vector2f (dx, dy); //basically declaring initial velocity
	}

};

class Universe {

	int N; //Number of planets 
	std::vector<Planet> planets; 
	float G = 10000.0, dist;
	Vector2f dpos, force;
	double KE, PE;

	public:
	RenderWindow space;

	Universe() { 
		N=0;
	}

	Universe(int n, bool random) { 
		N=n;
		if(random == true) {
			for(int i=0; i<n; i++) 
			planets.push_back(Planet());
		}
	}

	Universe(int m, Planet* list, int n) {
		N=n;
		for(int i=0; i<m; i++) 
			planets.push_back(list[i]);
		for(int i=0; i<n-m; i++) 
			planets.push_back(Planet());
	}

	void setupSpace() { 
		space = RenderWindow(VideoMode(WINDOW_SIZE), "Basic N-body system"); //Fullscreen support removed
		space.setFramerateLimit(144);
	}

	void show() {
		space.clear(Color::Black); 

		for(int i=0; i<N; i++) {
			planets.at(i).shape.setPosition({planets.at(i).pos.x, planets.at(i).pos.y});
			space.draw(planets.at(i).shape); 
		}

		space.display();
	}

	void simulate() {
		//Reset
		PE = 0, KE = 0;
		for(int i=0; i<N; i++) {
			planets.at(i).acc.x = 0;
			planets.at(i).acc.y = 0;
		}
		
		//Calculate interactions 
		for(int i=0; i<N; i++) {
			for(int j=i+1; j<N; j++) {
				//~~GRAVITY~~//
				dpos = planets.at(i).pos - planets.at(j).pos;
				dist = dpos.length();
				force = ((G*planets.at(i).mass*planets.at(j).mass)/(dist*dist*dist)) * dpos; //F_bar = (Gm1m2/r^3) r_bar
				if (force.length() > FORCE_MAX) force = FORCE_MAX * force.normalized();
				PE += -(G*planets.at(i).mass*planets.at(j).mass)/dist; // PE = -Gm1m2/r
				
				planets.at(i).acc += -force/planets.at(i).mass;
				planets.at(j).acc += force/planets.at(j).mass;
			}
		}

		//Update all planets
		for(int i=0; i<N; i++) {
			planets.at(i).updatePosVel();
			KE += 0.5*planets.at(i).mass*planets.at(i).vel.lengthSquared();
		}
	}

	double getKE() {
		return KE;
	}

	double getPE() {
		return PE;
	}

	double getTE() {
		return KE+PE;
	}

};

int main() {
	//---INITIALISATION---//

	// Planet earth(H_SIZE/2, V_SIZE/2, 100, Color::Blue, 2000);
	// Planet moon(H_SIZE/2 - 400, V_SIZE/2, 30, Color::White, 250);
	// // Planet rock(H_SIZE/2 - 60, V_SIZE/2 - 200, 10, Color::Cyan, 100);
	// earth.initialNudge(0, 0.25);
	// moon.initialNudge(0, -2);
	// // rock.initialNudge(5, 0);
	// Planet planets[2] = {earth, moon};
	// Universe universe (2, planets, 3);

	Universe universe(3, true);

	//---SETUP---//
	universe.setupSpace(); 
	
	std::vector<double> KineticEnergy, PotentialEnergy, TotalEnergy; //for plotting
	unsigned int count=0, C=25; //C is resolution of energy plot
	
	while(universe.space.isOpen()) {

		while(const std::optional event = universe.space.pollEvent()) {
			if(event->is<Event::Closed>()) 
				universe.space.close();
			else if(const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
				if(keyPressed->scancode == Keyboard::Scancode::Escape) 
					universe.space.close();
			}
		}

		//---DISPLAY CYCLE---//

		//clear screen & draw shapes
		universe.show();

		//simulate next time step
		universe.simulate();

		if(count==C) {
			count-=C;
			PotentialEnergy.push_back(universe.getPE());
			KineticEnergy.push_back(universe.getKE());
			TotalEnergy.push_back(universe.getTE());
		}else count++;
	}

	std::cout << "Simulation Ended! Plotting energies" << std::endl;

	{ using namespace matplot;
	plot(PotentialEnergy, "--xr");
	hold(on);
	plot(KineticEnergy, "--xgs");
	plot(TotalEnergy, "--:ks");
	show(); }

}
