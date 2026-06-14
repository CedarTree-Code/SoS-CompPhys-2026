#include "quadtree.h"

Node::Node(float w, float h, Vector2f midpt, int l, std::vector<Vector2f> pts) {
    hsize = w; vsize = h; lvl = l; 
    midpoint = midpt;
    points = pts;
    has_subnodes = false;
}

void Node::make() {
    has_subnodes = true;
    subnode[0] = new Node(hsize/2, vsize/2, Vector2f(midpoint.x - hsize/4, midpoint.y - vsize/4), lvl+1);
    subnode[1] = new Node(hsize/2, vsize/2, Vector2f(midpoint.x + hsize/4, midpoint.y - vsize/4), lvl+1);
    subnode[2] = new Node(hsize/2, vsize/2, Vector2f(midpoint.x - hsize/4, midpoint.y + vsize/4), lvl+1);
    subnode[3] = new Node(hsize/2, vsize/2, Vector2f(midpoint.x + hsize/4, midpoint.y + vsize/4), lvl+1);

    for(Vector2f p : points) {
        int k=0;
        if(p.x > midpoint.x) ++k;
        if(p.y > midpoint.y) ++++k; 
        subnode[k]->points.push_back(p);
    }

    for(int i=0; i<4; i++) 
        if (subnode[i]->points.size() > MAX_POINT_COUNT)
            subnode[i]->make();
}

void Node::draw(RenderWindow& window) {

    RectangleShape box({hsize, vsize});
    box.setOrigin({hsize/2, vsize/2});
    box.setFillColor(Color::Transparent);
    box.setOutlineThickness(-BOX_THICKNESS);

    box.setPosition({midpoint.x, midpoint.y});
    window.draw(box);

    if(has_subnodes == true)
        for(int i=0; i<4; i++) 
            subnode[i]->draw(window);
}
