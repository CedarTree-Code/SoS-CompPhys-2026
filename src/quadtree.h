#pragma once

#include <SFML/Graphics.hpp>
#include <set>

#include "planet.h"

using namespace sf;

class Node {
    std::set<int> indices;
    float size;
    Vector2f midpoint;
    Vector3f COM; //xpos, ypos, totalmass
    bool has_subnodes;
    Node* subnode[4];

    public:
    Node();
    Node(int n);
    Node(float s, int n);
    Node(Vector2f midpt, float s, int n = 0);

    void resize(int n, std::vector<Planet>& planets);
    void calcCOMs(std::vector<Planet>& planets);
    void splitNode(std::vector<Vector2f>& points);
    void splitNode(std::vector<Planet>& planets);
    void drawTree(RenderWindow& window);

    std::vector<Vector3f> approx (int b, std::vector<Planet>& planets);
    std::vector<Vector3f> check(int b, std::vector<Planet>& planets);

    ~Node();
};