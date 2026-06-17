#include <iostream>
#include <cmath>
#include <matplot/matplot.h>

#include "universe.h"

int main() {
	
	//---INITIALISATION---//
	RenderWindow space(VideoMode(WINDOW_SIZE), "Basic N-body system", SCREEN_STATE); //Fullscreen support removed
	space.setFramerateLimit(144);
	RenderWindow& ref = space;

	// Planet earth(.6f, .3f, 50, Color::Blue, 2000);
	// Planet moon(.2f, .3f, 10, Color::White, 500);
	// // Planet rock(H_SIZE/2 - 60, V_SIZE/2 - 200, 10, Color::Cyan, 100);
	// earth.initialNudge(0, 0.00125);
	// moon.initialNudge(0, -.005);
	// // rock.initialNudge(5, 0);
	// Planet planets[2] = {earth, moon};
	// Universe universe (2, planets, 2);

	Universe universe(500, true);

	//---SETUP---//
	std::vector<double> KineticEnergy, PotentialEnergy, TotalEnergy; //for plotting
	unsigned int count=0, C=24; //C is resolution of energy plot
	
	while(space.isOpen()) {

		while(const std::optional event = space.pollEvent()) {
			if(event->is<Event::Closed>()) 
				space.close();
			else if(const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
				if(keyPressed->scancode == Keyboard::Scancode::Escape) 
					space.close();
			}
		}

		//---DISPLAY CYCLE---//

		//clear screen
		space.clear(Color::Black);

        //draw shapes
		universe.show(ref);

		//simulate next time step
		universe.simulate(SIMULATION_TYPE);

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
