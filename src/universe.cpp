#include "universe.h"

Universe::Universe() { 
    N=0;
    qtree = Node();
}

Universe::Universe(int n, bool random) { 
    N=n;
    if(random == true) 
        for(int i=0; i<n; i++) 
            planets.push_back(Planet());
    qtree = Node(N);
}

Universe::Universe(int m, Planet* list, int n) {
    N=n;
    for(int i=0; i<m; i++) 
        planets.push_back(list[i]);
    for(int i=0; i<n-m; i++) 
        planets.push_back(Planet());
    qtree = Node(N);
}

Universe::Universe(const Universe& u) {
    N = u.N;
    planets = u.planets;
    qtree = Node(N);
}

void Universe::addPlanet(Planet p) {
    N++;
    planets.push_back(p);
    qtree = Node(N);
}

void Universe::addPlanets(int m, Planet* list) {
    N+=m;
    for(int i=0; i<m; i++) 
        planets.push_back(list[i]);
    qtree = Node(N);
}

void Universe::show(RenderWindow& space) {
    // space.clear(Color::Black); 

    for(int i=0; i<N; i++) {
        float x = deNormalize(planets.at(i).pos.x), y = deNormalize(planets.at(i).pos.y);
        planets.at(i).shape.setPosition({x, y});
        space.draw(planets.at(i).shape); 
    }

    space.display();
}

void Universe::simulate(int simtype, float Z) {
    //Timer timer("Universe::simulate()");
    //Reset
    PE = 0, KE = 0;
    for(int i=0; i<N; i++) {
        planets.at(i).acc.x = 0;
        planets.at(i).acc.y = 0;
    }

    if(simtype == 1) {
        //Calculate interactions 
        for(int i=0; i<N; i++) {
            for(int j=i+1; j<N; j++) {
                //~~GRAVITY~~//
                dpos = planets.at(i).pos - planets.at(j).pos;
                dist = dpos.length();
                mindist = std::sqrt(G*planets.at(i).mass*planets.at(j).mass/MAX_FORCE);
                force = ((G*planets.at(i).mass*planets.at(j).mass)/(dist*dist*dist)) * dpos; //F_bar = (Gm1m2/r^3) r_bar
                if (force.length() > MAX_FORCE) force = MAX_FORCE * force.normalized();
                
                if (dist > mindist) PE += -(G*planets.at(i).mass*planets.at(j).mass)/dist; // PE = -Gm1m2/r
                else PE += -2*std::sqrt(G*planets.at(i).mass*planets.at(j).mass*MAX_FORCE) + MAX_FORCE*dist;

                planets.at(i).acc += -force/planets.at(i).mass;
                planets.at(j).acc += force/planets.at(j).mass;
            }
        }

    }else if(simtype == 2){
        //setup
        qtree.resize(planets);
        nodepos = qtree.splitNode(planets);
        qtree.calcCOMs(planets);

        // { 
        // Timer timer("Force Calcs");
        for(int i=0; i<N; i++) {
            //compute forces Vector3f(xpos, ypos, mass)
            std::vector<Vector3f> approx = qtree.check(i, planets, Z);

            //Calculate interactions
            for(Vector3f a : approx) {
                dpos = planets.at(i).pos - Vector2f(a.x, a.y);
                dist = dpos.length();
                mindist = std::sqrt(G*planets.at(i).mass*a.z/MAX_FORCE);
                force = ((G*planets.at(i).mass*a.z)/(dist*dist*dist)) * dpos; //F_bar = (Gm1m2/r^3) r_bar
                if (force.length() > MAX_FORCE) force = MAX_FORCE * force.normalized();
                
                if (dist > mindist) PE += -(G*planets.at(i).mass*a.z)/dist; // PE = -Gm1m2/r
                else PE += -2*std::sqrt(G*planets.at(i).mass*a.z*MAX_FORCE) + MAX_FORCE*dist;

                planets.at(i).acc += -force/planets.at(i).mass;
            }

        // }
        PE/=2; //double calc-ed PE of system (probably cant calc PE like this here anyway)
        }

        //cleanup
        clearTree();
    }

    //Update all planets
        for(int i=0; i<N; i++) {
            planets.at(i).updatePosVel();
            KE += 0.5*planets.at(i).mass*planets.at(i).vel.lengthSquared();
        }

}

void Universe::createTree(RenderWindow& space) {
    qtree.resize(planets);
    qtree.splitNode(planets);
    qtree.drawTree(space);
}

void Universe::clearTree() {
    //Timer timer("Universe::clearTree()");
    for(Node* s : nodepos) delete s;
    nodepos.clear();
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