#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace sf;

struct Planet {

	float Xpos, Ypos, Xvel, Yvel, Xacc, Yacc, size;
	Color colour;
	double mass;
	CircleShape shape;

	Planet() {
		Xpos = Ypos = Xvel = Yvel = Xacc = Yacc = size = 0.f; 
		colour = Color::White; 
		mass = 0.0;
		shapeSetup();
	}

	Planet(float xpos, float ypos, float xvel, float yvel, float xacc, float yacc, float S, Color C, double Mass) {
		Xpos = xpos; Ypos = ypos; //initial position
		Xvel = xvel; Yvel = yvel; //initial velocity
		Xacc = xacc; Yacc = yacc; //initial acceleration
		mass = Mass;
		size = S; colour = C;
		shapeSetup(); //setup Planet shape for display
	}


	void shapeSetup(){
		shape.setRadius(size);
		shape.setOrigin({size, size});
		shape.setFillColor(colour);
	}

	void updatePositionVelocity(float dt) {
		Xvel += Xacc * dt; 
		Yvel += Yacc * dt;
		Xpos += Xvel * dt;
		Ypos += Yvel * dt;
	}

};

int main() {

	//----SETUP----//

	//RenderWindow window(VideoMode::getDesktopMode(), "Basic N-body system", State::Fullscreen);
	RenderWindow window(VideoMode({1600, 900}), "Basic N-body system");
	window.setFramerateLimit(60);
	float width = window.getSize().x;
	float height = window.getSize().y;

	int N=3;
	Planet earth(width/2, height/2, 0, -25, 0, 0, 100, Color::Blue, 4000);
	Planet moon(width/2 - 300, height/2, 0, 500, 0, 0, 20, Color::White, 250);
	Planet rock(width/2, height/2 - 500, -500, 0, 0, 0, 70, Color::Cyan, 100);
	
	Planet planets[N] = {earth, moon, rock}; 

	double G = 10000.0, dx, dy, dist, force, angle;

	Clock clock; //start timing
	Time time1 = clock.getElapsedTime(), time2, Dtime; 	//Dtime is (Delta) change in time
	
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
			planets[i].shape.setPosition({planets[i].Xpos, planets[i].Ypos});
			window.draw(planets[i].shape); 
		}

		//update time and objects
		time2 = clock.getElapsedTime();
		Dtime = time2-time1;
		time1 = time2;

		//update Net Accelerations
		for(int i=0; i<N; i++) {
			for(int j=i+1; j<N ; j++) {

				//~~GRAVITY~~//
				dx = planets[i].Xpos - planets[j].Xpos; dy = planets[i].Ypos - planets[j].Ypos;
				dist = std::sqrt(dx*dx + dy*dy);
				force = (G*planets[i].mass*planets[j].mass)/(dist*dist); //F = Gm1m2/r^2
				if (force > 300000) force = 300000;
				angle = std::atan2(dy, dx);
				
				planets[i].Xacc += -force*std::cos(angle)/planets[i].mass;
				planets[i].Yacc += -force*std::sin(angle)/planets[i].mass;
				planets[j].Xacc += force*std::cos(angle)/planets[j].mass;
				planets[j].Yacc += force*std::sin(angle)/planets[j].mass;

			}
		}

		for(int i=0; i<N; i++) planets[i].updatePositionVelocity(Dtime.asSeconds());

		window.display();

	}

	std::cout << "Window closed!" << std::endl;

}
