#pragma once 

#include "dcel.hpp"
#include "util.hpp"

struct Node {
    HalfEdgeId key;
    Node *left;
    Node *right;
    
    Node(HalfEdgeId key);
};

class Tree {
    DCEL &dcel;
    Node *root = nullptr;

public:
    Tree(DCEL &dcel);
    void print();
    void insert(HalfEdgeId key);
    void remove(HalfEdgeId key);
    HalfEdgeId get(vec2 point);
    bool pointToTheRightOfEdge(vec2 point, HalfEdgeId he);
    bool edgeToTheRightOfEdge(HalfEdgeId a, HalfEdgeId b);
};