#pragma once

#include <SFML/Graphics.hpp>
#include <set>

#include "planet.h"

using namespace sf;

class Node {
    std::vector<int> indices;
    float size;
    Vector2f midpoint;
    Vector3f COM; //xpos, ypos, totalmass
    bool has_subnodes, is_root = false;
    Node* subnode[4];

    public:
    Node();
    Node(int n);
    Node(float s, int n);
    Node(Vector2f midpt, float s);

    void resize(std::vector<Planet>& planets); //rescales bounding box
    void calcCOMs(std::vector<Planet>& planets);
    // void splitNode(std::vector<Vector2f>& points);
    std::vector<Node*> splitNode(std::vector<Planet>& planets);
    void drawTree(RenderWindow& window);

    std::vector<Vector3f> check(int b, std::vector<Planet>& planets, float Z);
};