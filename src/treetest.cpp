#include "quadtree.h"
#include "universe.h"

int main() {
    RenderWindow window(VideoMode(WINDOW_SIZE), "Quadtree", SCREEN_STATE);
    window.setFramerateLimit(144);
    RenderWindow& ref = window;

    Universe test(60, true);
    std::vector<Vector2f> points = test.getPlanetPositions();
    Node tree(H_SIZE, V_SIZE, Vector2f(H_SIZE/2, V_SIZE/2), 0, points);
    tree.make();
    tree.draw(ref);
    test.show(ref);

    while(window.isOpen()) {
		while(const std::optional event = window.pollEvent()) {
			if(event->is<Event::Closed>()) 
				window.close();
			else if(const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
				if(keyPressed->scancode == Keyboard::Scancode::Escape) 
					window.close();
			}
		}
    }
}