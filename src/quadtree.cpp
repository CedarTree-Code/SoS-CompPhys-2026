#include "quadtree.h"

Node::Node() {
    size = 1; 
    midpoint = Vector2f(.5f, .5f*((float)std::min(H_SIZE, V_SIZE)/N_SIZE));
    has_subnodes = false;
}

Node::Node(int n) {
    size = 1; 
    midpoint = Vector2f(.5f, .5f*((float)std::min(H_SIZE, V_SIZE)/N_SIZE));
    indices.reserve(n);
    for(int i=0; i<n; i++) 
        indices.push_back(i);
    has_subnodes = false;
}

Node::Node(float s, int n) {
    size = s; 
    midpoint = Vector2f(.5f, .5f*((float)std::min(H_SIZE, V_SIZE)/N_SIZE));
    indices.reserve(n);
    for(int i=0; i<n; i++) 
        indices.push_back(i);
    has_subnodes = false;
}

Node::Node(Vector2f midpt, float s) {
    size = s; 
    midpoint = midpt;
    has_subnodes = false;
}

void Node::resize(std::vector<Planet>& planets) {
    //Timer timer("Node::resize()");
    float newsize = 0.01, temp, Dx, Dy;
    for(int i : indices) {
        Dx = std::abs(midpoint.x - planets.at(i).pos.x);
        Dy = std::abs(midpoint.y - planets.at(i).pos.y);
        temp = 2*std::max(Dx, Dy);
        if(temp > newsize) newsize = temp;
    }
    size = newsize; is_root = true;
}

void Node::calcCOMs(std::vector<Planet>& planets) {
    //if(is_root) Timer timer("Node::calcCOMs()");
    float M=0, Xsum=0, Ysum=0;
    if(has_subnodes){
        for(int i=0; i<4; i++) {
            subnode[i]->calcCOMs(planets);
            M += subnode[i]->COM.z;
            Xsum += subnode[i]->COM.z*subnode[i]->COM.x;
            Ysum += subnode[i]->COM.z*subnode[i]->COM.y;        
        }
    }else{
        for(int i : indices) {
            M += planets.at(i).mass;
            Xsum += planets.at(i).mass*planets.at(i).pos.x;
            Ysum += planets.at(i).mass*planets.at(i).pos.y;
        }
    }
    if(M==0) COM = Vector3f(0, 0, 0);
    else COM = Vector3f(Xsum/M, Ysum/M, M);
}

// void Node::splitNode(std::vector<Vector2f>& things) { //points
//     has_subnodes = true;
//     subnode[0] = new Node(Vector2f(midpoint.x - size/4, midpoint.y - size/4), size/2);
//     subnode[1] = new Node(Vector2f(midpoint.x + size/4, midpoint.y - size/4), size/2);
//     subnode[2] = new Node(Vector2f(midpoint.x - size/4, midpoint.y + size/4), size/2);
//     subnode[3] = new Node(Vector2f(midpoint.x + size/4, midpoint.y + size/4), size/2);

//     for(int i : indices) {
//         int k=0;
//         if(things.at(i).x > midpoint.x) ++k;
//         if(things.at(i).y > midpoint.y) ++++k; 
//         subnode[k]->indices.insert(i);
//     }

//     for(int i=0; i<4; i++) 
//         if (subnode[i]->indices.size() > MAX_PER_NODE_COUNT)
//             subnode[i]->splitNode(things);
// }

std::vector<Node*> Node::splitNode(std::vector<Planet>& things) { //planets
    //if(is_root) Timer timer("Node::splitNode()");
    has_subnodes = true;
    subnode[0] = new Node(Vector2f(midpoint.x - size/4, midpoint.y - size/4), size/2);
    subnode[1] = new Node(Vector2f(midpoint.x + size/4, midpoint.y - size/4), size/2);
    subnode[2] = new Node(Vector2f(midpoint.x - size/4, midpoint.y + size/4), size/2);
    subnode[3] = new Node(Vector2f(midpoint.x + size/4, midpoint.y + size/4), size/2);
    
    std::vector<Node*> out, temp;
    for(int i=0; i<4; i++) out.push_back(subnode[i]);

    for(int i : indices) {
        int k=0;
        if(things.at(i).pos.x > midpoint.x) ++k;
        if(things.at(i).pos.y > midpoint.y) ++++k; 
        subnode[k]->indices.push_back(i);
    }

    for(int i=0; i<4; i++) 
        if (subnode[i]->indices.size() > MAX_PER_NODE_COUNT) {
            temp = subnode[i]->splitNode(things);
            out.insert(out.end(), temp.begin(), temp.end());
        }

    return out;
}

void Node::drawTree(RenderWindow& window) {

    float s = deNormalize(size);

    RectangleShape box({s, s});
    box.setOrigin({s/2, s/2});
    box.setFillColor(Color::Transparent);
    box.setOutlineThickness(-BOX_THICKNESS);

    box.setPosition({deNormalize(midpoint.x), deNormalize(midpoint.y)});
    window.draw(box);

    if(has_subnodes == true)
        for(int i=0; i<4; i++) 
            subnode[i]->drawTree(window);
}

std::vector<Vector3f> Node::check(int b, std::vector<Planet>& planets, float Z) {
    std::vector<Vector3f> out, temp;
    Vector2f disp = Vector2(COM.x, COM.y) - planets.at(b).pos;
    float dist = disp.length();
    if(size/dist < Z) { 
        out.push_back(COM);
    }else{
        if(has_subnodes) {
            for(int i=0; i<4; i++) {
                temp = subnode[i]->check(b, planets, Z);
                out.insert(out.end(), temp.begin(), temp.end());
            }
        }else{
            for(int i : indices) {
                if(i!=b) out.push_back(Vector3f(planets.at(i).pos.x, planets.at(i).pos.y, planets.at(i).mass));
            }
        }
    }
    return out;
}

// void Node::clean() {
//     if (has_subnodes) {
//         for(int i=0; i<4; i++){
//             if(subnode[i]->has_subnodes) {
//                 bool check = false;
//                 for(int j=0; j<4; j++) if(subnode[i]->subnode[j]->has_subnodes) check=true;
//                 if(!check) {
//                     for(int j=0; j<4; j++) {
//                         delete subnode[i]->subnode[j];
//                         //subnode[i]->subnode[j] = nullptr;
//                     }
//                     subnode[i]->has_subnodes = false;
//                 }else{
//                     subnode[i]->clean();
//                 }
//             }
//             delete subnode[i];
//             //subnode[i] = nullptr;
//         }
//         has_subnodes = false;
//     }
// }