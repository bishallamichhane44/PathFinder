#include<iostream>
#include <windows.h>
#include<math.h>
#include <SFML/Graphics.hpp>



const int FLT_MAX=10;
class Node {
public:
    int x, y;
    float gCost, hCost, fCost;
    bool open, close, visited,blocked=false;
    Node* parent;
    std::vector<Node*> neighbours;

    Node(int x, int y) : x(x), y(y), gCost(FLT_MAX), hCost(FLT_MAX), fCost(FLT_MAX), parent(nullptr), open(false), close(false), visited(false) {}

    void calculateCost(Node* startNode,Node* endNode) {
        // Assuming a uniform cost, update this if the cost is variable
        gCost = std::hypot(x - startNode->x, y - startNode->y);
        hCost = std::hypot(x - endNode->x, y - endNode->y);
        fCost = gCost + hCost;
    }

    void addNeighbour(Node* neighbour) {
        neighbours.push_back(neighbour);
    }
};

class NodeContainer {
public:
    std::vector<std::vector<Node>> nodes;
    Node* startNode;
    Node* endNode;

    NodeContainer(int rows, int columns) : startNode(nullptr), endNode(nullptr) {
        nodes.reserve(rows);
        for (int i = 0; i < rows; ++i) {
            nodes.push_back(std::vector<Node>());
            nodes[i].reserve(columns);
            for (int j = 0; j < columns; ++j) {
                nodes[i].emplace_back(j, i);
            }
        }
    }

    void setStartNode(int x, int y) {
        startNode = &nodes[y][x];
        startNode->open = true; // Start node is always open to begin with
    }

    void setEndNode(int x, int y) {
        endNode = &nodes[y][x];
    }

    Node* getNode(int x, int y) {
        return &nodes[y][x];
    }
};
