#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace sf;

struct Planet {

	float Xpos, Ypos, prevXpos, prevYpos, Xvel, Yvel, Xacc, Yacc, size;
	Color colour;
	double mass;
	CircleShape shape;

	Planet() {
		Xpos = Ypos = prevXpos = prevYpos = Xacc = Yacc = size = 0.f; 
		colour = Color::White; 
		mass = 0.0;
		shapeSetup();
	}

	Planet(float xpos, float ypos, float xacc, float yacc, float S, Color C, double Mass) {
		Xpos = xpos; Ypos = ypos; //initial position
		Xacc = xacc; Yacc = yacc; //initial acceleration
		mass = Mass;
		size = S; colour = C;
		prevXpos = xpos; prevYpos = ypos;
		shapeSetup(); //setup Planet shape for display
	}


	void shapeSetup(){
		shape.setRadius(size);
		shape.setOrigin({size, size});
		shape.setFillColor(colour);
	}

	void updatePositions(float dt) {
		//Verlet integration//
		float Xtemp = Xpos, Ytemp = Ypos;
		Xpos = 2*Xpos - prevXpos + Xacc*dt*dt; //O(t^4) error
		Ypos = 2*Ypos - prevYpos + Yacc*dt*dt;
		Xvel = (Xpos - prevXpos)/2*dt; //O(t^2) error
		Yvel = (Ypos - prevYpos)/2*dt;
		prevXpos = Xtemp; prevYpos = Ytemp;
	}

};

int main() {

	//----SETUP----//

	//RenderWindow window(VideoMode::getDesktopMode(), "Basic N-body system", State::Fullscreen);
	RenderWindow window(VideoMode({1600, 900}), "Basic N-body system");
	window.setFramerateLimit(60);
	float width = window.getSize().x;
	float height = window.getSize().y;

	int N=2;
	Planet earth(width/2, height/2, 0, 0, 100, Color::Blue, 2000);
	Planet moon(width/2 - 400, height/2, 0, 0, 20, Color::White, 250);
	earth.prevYpos = height/2 + 0.25;
	moon.prevYpos = height/2 - 2;
	//Planet rock(width/2, height/2 - 500, 0, 0, 70, Color::Cyan, 100);

	Planet planets[N] = {earth, moon}; 

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
			planets[i].Xacc=0;
			planets[i].Yacc=0;
		}
		
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

		for(int i=0; i<N; i++) planets[i].updatePositions(Dtime.asSeconds());

		window.display();

	}

	std::cout << "Window closed!" << std::endl;

}
