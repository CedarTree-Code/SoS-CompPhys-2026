#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <matplot/matplot.h>
using namespace sf;

struct Planet {

	Vector2f pos, prevpos, vel, acc; 
	float size, mass;
	Color colour;
	CircleShape shape;

	Planet() {
		pos.x = pos.y = prevpos.x = prevpos.y = acc.x = acc.y = size = mass = 0; 
		colour = Color::White; 
		shapeSetup();
	}

	Planet(float PosX, float PosY, float AccX, float AccY, float S, Color C, float Mass) {
		pos.x = PosX; pos.y = PosY; //initial position
		acc.x = AccX; acc.y = AccY; //initial acceleration
		mass = Mass;
		size = S; colour = C;
		prevpos = pos;
		shapeSetup(); //setup Planet shape for display
	}


	void shapeSetup(){
		shape.setRadius(size);
		shape.setOrigin({size, size});
		shape.setFillColor(colour);
	}

	void updatePositions(float dt) {
		//Verlet integration//
		Vector2f temp = pos;
		pos = 2.f*pos - prevpos + acc*dt*dt; //O(t^4) error
		vel = (pos - prevpos)/(2*dt); //O(t^2) error
		prevpos = temp;
	}

};

class Universe {
	public:

};

int main() {

	//----SETUP----//

	//RenderWindow window(VideoMode::getDesktopMode(), "Basic N-body system", State::Fullscreen);
	RenderWindow window(VideoMode({1600, 900}), "Basic N-body system");
	window.setFramerateLimit(60);
	float width = window.getSize().x;
	float height = window.getSize().y;

	int N=3;
	Planet earth(width/2, height/2, 0, 0, 100, Color::Blue, 2000);
	Planet moon(width/2 - 400, height/2, 0, 0, 30, Color::White, 250);
	Planet rock(width/2 - 60, height/2 - 200, 0, 0, 10, Color::Cyan, 100);
	earth.prevpos = earth.pos + Vector2f (0, 0.25);
	moon.prevpos = moon.pos + Vector2f (0, -2);
	rock.prevpos = rock.pos + Vector2f (5, 0);

	Planet planets[N] = {earth, moon, rock}; 

	float G = 10000.0, dist;
	Vector2f dpos, force;

	float Dtime = 1./80; //1./fps

	std::vector<double> KineticEnergy, PotentialEnergy, TotalEnergy;
	double KE, PE;
	
	while(window.isOpen()) {

		while(const std::optional event = window.pollEvent()) {
			if(event->is<Event::Closed>()) 
				window.close();
			else if(const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
				if(keyPressed->scancode == Keyboard::Scancode::Escape) 
					window.close();
			}
		}

		//----DISPLAY CYCLE & CALCULATIONS----//

		//clear screen
		window.clear(Color::Black); 

		//draw shapes
		for(int i=0; i<N; i++) {
			planets[i].shape.setPosition({planets[i].pos.x, planets[i].pos.y});
			window.draw(planets[i].shape); 
		}

		//update Net Accelerations
		for(int i=0; i<N; i++) {
			planets[i].acc.x = 0;
			planets[i].acc.y = 0;
		}

		PE = 0, KE = 0;
		
		for(int i=0; i<N; i++) {
			for(int j=i+1; j<N ; j++) {

				//~~GRAVITY~~//
				dpos = planets[i].pos - planets[j].pos;
				dist = dpos.length();
				force = ((G*planets[i].mass*planets[j].mass)/(dist*dist*dist)) * dpos; //F = Gm1m2/r^2
				if (force.length() > 300000.f) force = 300000.f * force.normalized();
				PE += -(G*planets[i].mass*planets[j].mass)/dist; // -Gm1m2/r
				
				planets[i].acc += -force/planets[i].mass;
				planets[j].acc += force/planets[j].mass;

			}
		}

		for(int i=0; i<N; i++) {
			planets[i].updatePositions(Dtime);
			KE += 0.5*planets[i].mass*planets[i].vel.lengthSquared();
		}

		PotentialEnergy.push_back(PE);
		KineticEnergy.push_back(KE);
		TotalEnergy.push_back(KE+PE);

		window.display();

	}

	std::cout << "Simulation Ended! - Energy Plot" << std::endl;

	{ 
		using namespace matplot;
		plot(PotentialEnergy, "--xr");
		hold(on);
		plot(KineticEnergy, "--xgs");
		plot(TotalEnergy, "--:ks");
		show();
	}

}
