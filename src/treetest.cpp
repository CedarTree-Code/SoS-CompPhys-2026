#include "quadtree.h"
#include "universe.h"

int main() {
  RenderWindow window(VideoMode(WINDOW_SIZE), "Quadtree", SCREEN_STATE);
  window.setFramerateLimit(144);
  RenderWindow& ref = window;

  int N=20;
  Universe test(N, true);

  test.createTree(ref);
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

  //randomSeedGen(20);
}