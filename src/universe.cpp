#include "universe.h"

Universe::Universe() { 
    N=0;
}

Universe::Universe(int n, bool random) { 
    N=n;
    if(random == true) 
        for(int i=0; i<n; i++) 
            planets.push_back(Planet());
}

Universe::Universe(int m, Planet* list, int n) {
    N=n;
    for(int i=0; i<m; i++) 
        planets.push_back(list[i]);
    for(int i=0; i<n-m; i++) 
        planets.push_back(Planet());
}

void Universe::addPlanet(Planet p) {
    N++;
    planets.push_back(p);
}

void Universe::addPlanets(int m, Planet* list) {
    N+=m;
    for(int i=0; i<m; i++) 
        planets.push_back(list[i]);
}

void Universe::show(RenderWindow& space) {
    space.clear(Color::Black); 

    for(int i=0; i<N; i++) {
        planets.at(i).shape.setPosition({planets.at(i).pos.x, planets.at(i).pos.y});
        space.draw(planets.at(i).shape); 
    }

    space.display();
}

void Universe::simulate() {
    //Reset
    PE = 0, KE = 0;
    for(int i=0; i<N; i++) {
        planets.at(i).acc.x = 0;
        planets.at(i).acc.y = 0;
    }
    
    //Calculate interactions 
    for(int i=0; i<N; i++) {
        for(int j=i+1; j<N; j++) {
            //~~GRAVITY~~//
            dpos = planets.at(i).pos - planets.at(j).pos;
            dist = dpos.length();
            mindist = std::sqrt(G*planets.at(i).mass*planets.at(j).mass/MAX_FORCE);
            force = ((G*planets.at(i).mass*planets.at(j).mass)/(dist*dist*dist)) * dpos; //F_bar = (Gm1m2/r^3) r_bar
            if (force.length() > MAX_FORCE) force = MAX_FORCE * force.normalized();
            
            PEij = -(G*planets.at(i).mass*planets.at(j).mass)/dist; // PE = -Gm1m2/r
            if (dist > mindist) PE += PEij;
            else PE += -2*std::sqrt(G*planets.at(i).mass*planets.at(j).mass*MAX_FORCE) + MAX_FORCE*dist;

            planets.at(i).acc += -force/planets.at(i).mass;
            planets.at(j).acc += force/planets.at(j).mass;
        }
    }

    //Update all planets
    for(int i=0; i<N; i++) {
        planets.at(i).updatePosVel();
        KE += 0.5*planets.at(i).mass*planets.at(i).vel.lengthSquared();
    }
}

double Universe::getKE() {
    return KE;
}

double Universe::getPE() {
    return PE;
}

double Universe::getTE() {
    return KE+PE;
}