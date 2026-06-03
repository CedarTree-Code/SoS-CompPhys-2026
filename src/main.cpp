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
		shape.setRadius(size);
		shape.setOrigin({size, size});
		shape.setFillColor(colour);
	}

	Planet(float xpos, float ypos, float xvel, float yvel, float xacc, float yacc, float S, Color C, double Mass) {
		Xpos = xpos; Ypos = ypos; //initial position
		Xvel = xvel; Yvel = yvel; //initial velocity
		Xacc = xacc; Yacc = yacc; //initial acceleration
		mass = Mass;
		size = S; colour = C;

		//setup Planet shape for display
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

	//RenderWindow window(VideoMode::getDesktopMode(), "Basic 2-body system", State::Fullscreen);
	RenderWindow window(VideoMode({1600, 900}), "Basic 2-body system");
	window.setFramerateLimit(60);
	float width = window.getSize().x;
	float height = window.getSize().y;

	Planet earth(width/2, height/2, 0, -25, 0, 0, 100, Color::Blue, 2000);
	Planet moon(width/2 - 300, height/2, 0, 200, 0, 0, 20, Color::White, 250);

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
		earth.shape.setPosition({earth.Xpos, earth.Ypos});
		moon.shape.setPosition({moon.Xpos, moon.Ypos});
		window.draw(earth.shape); 
		window.draw(moon.shape);

		//update time and object
		time2 = clock.getElapsedTime();
		Dtime = time2-time1;
		time1 = time2;

		//~~GRAVITY~~//
		dx = earth.Xpos - moon.Xpos; dy = earth.Ypos - moon.Ypos;
		dist = std::sqrt(dx*dx + dy*dy);
		force = (G*earth.mass*moon.mass)/(dist*dist); //F = Gm1m2/r^2
		if (force > 300000) force = 300000;
		angle = std::atan2(dy, dx);

		//update Accelerations
		earth.Xacc = -force*std::cos(angle)/earth.mass;
		earth.Yacc = -force*std::sin(angle)/earth.mass;
		moon.Xacc = force*std::cos(angle)/moon.mass;
		moon.Yacc = force*std::sin(angle)/moon.mass;

		earth.updatePositionVelocity(Dtime.asSeconds());
		moon.updatePositionVelocity(Dtime.asSeconds());

		window.display();

	}

	std::cout << "Window closed!" << std::endl;

}
