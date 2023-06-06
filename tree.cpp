#include "tree.hpp"

Node::Node(HalfEdgeId key)
    : key(key), left(nullptr), right(nullptr), removed(false)
{}

Tree::Tree(DCEL &dcel) : dcel(dcel) {};

bool Tree::pointToTheRightOfEdge(vec2 point, HalfEdgeId he) {
    // TODO: check if correct
    vec2 a = dcel.getVertex(dcel.origin(he)).coords - point;
    vec2 b = dcel.getVertex(dcel.origin(dcel.twin(he))).coords - point;

    return det(a, b) > 0;
}

// returns if a is to the right of b
bool Tree::edgeToTheRightOfEdge(HalfEdgeId a, HalfEdgeId b) {
    vec2 aCoord = dcel.getVertex(dcel.origin(a)).coords;
    vec2 aOpCoord = dcel.getVertex(dcel.origin(dcel.twin(a))).coords;
    
    vec2 bCoord = dcel.getVertex(dcel.origin(b)).coords;
    vec2 bOpCoord = dcel.getVertex(dcel.origin(dcel.twin(b))).coords;
    
    float maxY = std::max(aCoord.y, bCoord.y);
    float minY = std::min(aOpCoord.y, bOpCoord.y);
    
    // guaranteed to intersect the two edges in the X axis
    float y = (maxY + minY) / 2;
    
    // (y - y0) * ((x1 - x0) / (y1 - y0)) + x0 = x
    float ax = (y - aCoord.y) * ((aOpCoord.x - aCoord.x) / (aOpCoord.y - aCoord.y)) + aCoord.x;
    float bx = (y - bCoord.y) * ((bOpCoord.x - bCoord.x) / (bOpCoord.y - bCoord.y)) + bCoord.x;
    
    return ax > bx;
}

void Tree::insert(HalfEdgeId key) {
    if (root == nullptr) {
        root = new Node(key);
        return;
    }

    Node *current = root;

    while (true) {
        std::cout << "Insert Current " <<  current << std::endl;
        if (edgeToTheRightOfEdge(key, current->key)) {
            if (current->right == nullptr) {
                current->right = new Node(key);
                return;
            } else {
                current = current->right;
            }
        } else {
            if (current->left == nullptr) {
                current->left = new Node(key);
                return;
            } else {
                current = current->left;
            }
        }
    }
}

HalfEdgeId Tree::get(vec2 point)
{
    Node *current = root;
    HalfEdgeId best = -1;

    while (true)
    {
        if (current == nullptr)
            return best;
        
        if (pointToTheRightOfEdge(point, current->key))
        {
            best = current->key;
            current = current->right;
        }
        else
            current = current->left;
    }
}

void Tree::remove(HalfEdgeId key) {
    Node *current = root;

    while (true) {
        if (current == nullptr) {
            return;
        }

        if (current->key == key) {
            current->removed = true;
            return;
        }

        if (edgeToTheRightOfEdge(key, current->key)) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
}
