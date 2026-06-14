#pragma once

#include <SFML/Graphics.hpp>

#include "const.h"

using namespace sf;

class Node {
    std::vector<Vector2f> points;
    int lvl;
    float hsize, vsize;
    Vector2f midpoint;
    bool has_subnodes;
    Node* subnode[4];

    public:
    Node(float w, float h, Vector2f midpt, int l = 0, std::vector<Vector2f> pts = {});
    void make();
    void draw(RenderWindow& window);
};