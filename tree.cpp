#include "tree.hpp"

#include <stack>
#include <cassert>

Node::Node(HalfEdgeId key)
    : key(key), left(nullptr), right(nullptr)
{}

Tree::Tree(DCEL &dcel) : dcel(dcel) {};

bool Tree::pointToTheRightOfEdge(vec2 point, HalfEdgeId he) {
    // TODO: check if correct
    vec2 a = point - dcel.getVertex(dcel.origin(he)).coords;
    vec2 b = dcel.getVertex(dcel.origin(dcel.twin(he))).coords - dcel.getVertex(dcel.origin(he)).coords;

    return det(a, b) <= 0;
}

// returns if a is to the right of b
bool Tree::edgeToTheRightOfEdge(HalfEdgeId a, HalfEdgeId b) {
    vec2 aCoord = dcel.getVertex(dcel.origin(a)).coords;
    vec2 aOpCoord = dcel.getVertex(dcel.origin(dcel.twin(a))).coords;
    
    vec2 bCoord = dcel.getVertex(dcel.origin(b)).coords;
    vec2 bOpCoord = dcel.getVertex(dcel.origin(dcel.twin(b))).coords;
    
    double maxY = std::min(aCoord.y, bCoord.y);
    double minY = std::max(aOpCoord.y, bOpCoord.y);
    
    // guaranteed to intersect the two edges in a horizontal line
    double y = (maxY + minY) / 2;
    
    double ax = dcel.getXfromY(a, y);
    double bx = dcel.getXfromY(b, y);

    return ax > bx;
}

void Tree::insert(HalfEdgeId key) {
    if (root == nullptr) {
        root = new Node(key);
        return;
    }
    
    Node *current = root;

    while (true) {
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
            break;
        
        if (pointToTheRightOfEdge(point, current->key))
        {
            best = current->key;
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }

    if (best == -1)
    {
        std::cerr << "ERROR: could not find edge left to (" << point << ")" << std::endl;
        assert(false);
    }

    return best;
}

std::pair<Node *, Node *> minValueNode(Node *node)
{
    Node *current = node;
    Node *parent = nullptr;

    while (current && current->left != nullptr)
    {
        parent = current;
        current = current->left;
    }

    return {current, parent};
}

void Tree::remove(HalfEdgeId key)
{
    std::stack<std::pair<Node *, Node *>> s;

    Node *current = root;
    Node *parent = nullptr;
    s.push({current, parent});

    while (!s.empty())
    {
        current = s.top().first;
        parent = s.top().second;
        s.pop();

        if (!current)
            continue;

        if (current->key == key)
            break;
        
        if (edgeToTheRightOfEdge(key, current->key))
            s.push({current->right, current});
        else 
            s.push({current->left, current});
    }

    if (!current)
        return;
    
    if (!current->left && current->right)
    {
        Node *temp = current->right;
        current->key = temp->key;
        current->left = temp->left;
        current->right = temp->right;

        delete temp;
    }
    else if (!current->right && current->left)
    {
        Node *temp = current->left;

        current->key = temp->key;
        current->left = temp->left;
        current->right = temp->right;

        delete temp;
    }
    else if (!current->right && !current->left)
    {
        if (parent)
        {
            if (parent->left == current)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
        else
            root = nullptr;

        delete current;
    }
    else
    {
        std::pair<Node *, Node *> p = minValueNode(current->right);
        Node *min = p.first;
        Node *minParent = p.second;

        if (minParent)
            minParent->left = nullptr;
        else
            current->right = nullptr;

        current->key = min->key;

        delete min;
    }
}

void Tree::print()
{
    std::stack<std::tuple<Node *, char, int>> s;
    Node* current = root;
    int indent = 0;
    char side = 'O';

    s.push({current, side, indent});

    while (!s.empty())
    {
        current = std::get<0>(s.top());
        side = std::get<1>(s.top());
        indent = std::get<2>(s.top());
        s.pop();

        if (!current)
            continue;

        for (int i = 0; i < indent; i++)
            std::cerr << " ";

        std::cerr << side << " " << current->key << " " << dcel.getHalfEdge(current->key) << " " << dcel.getVertex(dcel.origin(current->key)).coords << std::endl;

        s.push({current->left, 'L', indent + 2});
        s.push({current->right, 'R', indent + 2});
    }
}
