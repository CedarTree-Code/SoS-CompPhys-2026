#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int main() {

	//setup

	//RenderWindow window(VideoMode::getDesktopMode(), "Drawing a circle", State::Fullscreen);
	RenderWindow window(VideoMode({1600, 900}), "Drawing a circle");
	float height = window.getSize().x;
	float width = window.getSize().y;

	CircleShape circle(100.f);
	circle.setOrigin({100.f, 100.f});
	circle.setFillColor(Color::Blue);

	float circleXpos = height/2, circleYpos = width/2; 	//initial position
	float circleXvel = 300.f, circleYvel = -500.f; 			//initial velocity

	Clock clock; 				//start timing

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

		//----DISPLAY CYCLE----

		//clear screen
		window.clear(Color::Black); 

		//draw object
		circle.setPosition({circleXpos, circleYpos});
		window.draw(circle); 

		//update object
		time2 = clock.getElapsedTime();
		Dtime = time2-time1;
		time1 = time2;
		circleXpos+=circleXvel*Dtime.asSeconds(); circleYpos+=circleYvel*Dtime.asSeconds();

		window.display();

	}

	std::cout << "Window closed!" << std::endl;

}
