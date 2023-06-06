#pragma once 

#include "dcel.hpp"
#include "util.hpp"

struct Node {
    HalfEdgeId key;
    Node *left;
    Node *right;
    bool removed;
    
    Node(HalfEdgeId key);
};

class Tree {
    DCEL &dcel;
    Node *root;
public:
    Tree(DCEL &dcel) : dcel(dcel) {};
    void insert(HalfEdgeId key);
    void remove(HalfEdgeId key);
    HalfEdgeId get(vec2 point);
    bool pointToTheRightOfEdge(vec2 point, HalfEdgeId he);
    bool edgeToTheRightOfEdge(HalfEdgeId a, HalfEdgeId b);
};