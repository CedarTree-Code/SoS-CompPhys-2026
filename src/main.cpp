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

	Universe universe(NO_OF_PLANETS, true);
	#ifdef TEST_ITER
	Universe u1 = universe, u2 = universe, u3 = universe;
	#endif
	
	//---SETUP---//
	std::vector<double> KineticEnergy, PotentialEnergy, TotalEnergy; //for plotting
	unsigned int count=0, C=12, i=0; //C is resolution of energy plot
	
	while(space.isOpen()) {

		while(const std::optional event = space.pollEvent()) {
			if(event->is<Event::Closed>()) 
				space.close();
			else if(const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
				if(keyPressed->scancode == Keyboard::Scancode::Escape) 
					space.close();
			}
		}

		#ifdef TEST_ITER

		{ Timer timer("the Normal algorithm"); //simtype 1
		for(; i<TEST_ITER; i++) { 
			//---DISPLAY CYCLE---//

			//clear screen
			space.clear(Color::Black);

			//draw shapes
			universe.show(ref);

			//simulate next time step
			universe.simulate(1);

			if(count==C) {
				count-=C;
				PotentialEnergy.push_back(universe.getPE());
				KineticEnergy.push_back(universe.getKE());
				TotalEnergy.push_back(universe.getTE());
			}else count++;
		} }

		{ Timer timer("the Barnes-Hut alg (Z = 0.1)"); //simtype 2 --- Z = 0.1
		for(; i<2*TEST_ITER; i++) { 
			//---DISPLAY CYCLE---//

			//clear screen
			space.clear(Color::Black);

			//draw shapes
			u1.show(ref);

			//simulate next time step
			u1.simulate(2, Z1);

			if(count==C) {
				count-=C;
				PotentialEnergy.push_back(universe.getPE());
				KineticEnergy.push_back(universe.getKE());
				TotalEnergy.push_back(universe.getTE());
			}else count++;
		} }

		{ Timer timer("the Barnes-Hut alg (Z = 0.5)");//simtype 2 --- Z = 0.5
		for(; i<3*TEST_ITER; i++) { 
			//---DISPLAY CYCLE---//

			//clear screen
			space.clear(Color::Black);

			//draw shapes
			u2.show(ref);

			//simulate next time step
			u2.simulate(2, Z2);

			if(count==C) {
				count-=C;
				PotentialEnergy.push_back(universe.getPE());
				KineticEnergy.push_back(universe.getKE());
				TotalEnergy.push_back(universe.getTE());
			}else count++;
		} }

		{ Timer timer("the Barnes-Hut alg (Z = 0.7)"); //simtype 2 --- Z = 0.7
		for(; i<4*TEST_ITER; i++) { 
			//---DISPLAY CYCLE---//

			//clear screen
			space.clear(Color::Black);

			//draw shapes
			u3.show(ref);

			//simulate next time step
			u3.simulate(2, Z3);

			if(count==C) {
				count-=C;
				PotentialEnergy.push_back(universe.getPE());
				KineticEnergy.push_back(universe.getKE());
				TotalEnergy.push_back(universe.getTE());
			}else count++;
		} }

		space.close();

		#else
		//---DISPLAY CYCLE---//

		//clear screen
		space.clear(Color::Black);

        //draw shapes
		universe.show(ref);

		//simulate next time step
		universe.simulate(SIMULATION_TYPE, Z);

		if(count==C) {
			count-=C;
			PotentialEnergy.push_back(universe.getPE());
			KineticEnergy.push_back(universe.getKE());
			TotalEnergy.push_back(universe.getTE());
		}else count++;

		#endif

	}

	std::cout << "Simulation Ended!" << std::endl;

	// { std::cout << "Plotting Energies..." << std::endl;
	// using namespace matplot;
	// plot(PotentialEnergy, "--xr");
	// hold(on);
	// plot(KineticEnergy, "--xgs");
	// plot(TotalEnergy, "--:ks");
	// show(); }

}
