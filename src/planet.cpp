#include "planet.h"

Planet::Planet() {
    //Random planet
    pos.x = fRandom(0, H_SIZE);
    pos.y = fRandom(0, V_SIZE);
    prevpos.x = pos.x + fRandom(-MAX_NUDGE, MAX_NUDGE);
    prevpos.y = pos.y + fRandom(-MAX_NUDGE, MAX_NUDGE);
    acc.x = acc.y = 0;
    size = fRandom(1, 150);
    mass = fRandom(100, 3000); 

    colour = Color(iRandom(0, 255), iRandom(0, 255), iRandom(0, 255)); 
    shapeSetup();
}

Planet::Planet(float PosX, float PosY, float S, Color C, float Mass) {
    pos.x = PosX; pos.y = PosY; //initial position
    acc.x = acc.y = 0; //initial acceleration
    mass = Mass;
    size = S; colour = C;
    prevpos = pos;
    shapeSetup(); //setup Planet shape for display
}


void Planet::shapeSetup() {
    shape.setRadius(size);
    shape.setOrigin({size, size});
    shape.setFillColor(colour);
}

void Planet::updatePosVel() {
    //Verlet integration//
    Vector2f temp = pos;
    pos = 2.f*pos - prevpos + acc*D_t*D_t; //O(t^4) error
    vel = (pos - prevpos)/(2*D_t); //O(t^2) error
    prevpos = temp;
}

void Planet::initialNudge(float dx, float dy) {
    prevpos = pos + Vector2f (dx, dy); 
}