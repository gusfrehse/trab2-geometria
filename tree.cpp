#include "tree.hpp"

bool Tree::pointToTheRightOfEdge(vec2 point, HalfEdgeId he) {
    vec2 a = dcel.getVertex(dcel.origin(he)).coords - point;
    vec2 b = dcel.getVertex(dcel.origin(dcel.twin(he))).coords - point;

    return det(a, b) < 0;
}

bool Tree::edgeToTheRightOfEdge(HalfEdgeId a, HalfEdgeId b) {
    vec2 aCoord = dcel.getVertex(dcel.origin(a)).coords;
    vec2 aOpCoord = dcel.getVertex(dcel.origin(dcel.twin(a))).coords;
    
    vec2 bCoord = dcel.getVertex(dcel.origin(b)).coords;
    vec2 bOpCoord = dcel.getVertex(dcel.origin(dcel.twin(b))).coords;
    
    float maxY = std::max(aCoord.y, bCoord.y);
    float minY = std::min(aOpCoord.y, bOpCoord.y);
    
    // guaranteed to intersect the two edges in the X axis
    float y = (maxY + minY) / 2;
    
    vec2 aVec = aCoord - aOpCoord;
}

void Tree::insert(HalfEdgeId key) {
    if (root == nullptr) {
        root = new Node(key);
        return;
    }

    Node *curr = root;
    Node *prev = nullptr;

    while (curr != nullptr) {
        prev = curr;
        if (pointToTheRightOfEdge(dcel.getVertex(dcel.origin(key)).coords, curr->key)) {
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }

    if (pointToTheRightOfEdge(dcel.getVertex(dcel.origin(key)).coords, prev->key)) {
        prev->right = new Node(key);
    } else {
        prev->left = new Node(key);
    }
}

void remove(HalfEdgeId key) {

}

