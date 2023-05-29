#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>

#include "util.hpp"

#include "dcel.hpp"

struct triangle {
    std::array<int, 3> vertices;
    std::array<int, 3> neighbours;
};

using edge = std::pair<int, int>;

void readVertices(int n, std::vector<vec2>& vertices) {
    vertices.resize(n);

    for (int i = 0; i < n; ++i) {
        vertices[i] = vec2::read();
    }
}

bool isSplitVertex(DCELVertex *v) {
    DCELVertex *prev = v->incidentEdge->prev->origin;
    DCELVertex *next = v->incidentEdge->twin->origin;

    vec2 a = prev->coords - v->coords;
    vec2 b = next->coords - v->coords;

    return (prev->coords.y <= v->coords.y && next->coords.y <= v->coords.y) && (det(a, b) > 0); 
}

bool isStartVertex(DCELVertex *v) {
    DCELVertex *prev = v->incidentEdge->prev->origin;
    DCELVertex *next = v->incidentEdge->twin->origin;

    vec2 a = prev->coords - v->coords;
    vec2 b = next->coords - v->coords;

    return (prev->coords.y <= v->coords.y && next->coords.y <= v->coords.y) && (det(a, b) <= 0); 
}

bool isMergeVertex(DCELVertex *v) {
    DCELVertex *prev = v->incidentEdge->prev->origin;
    DCELVertex *next = v->incidentEdge->twin->origin;

    vec2 a = prev->coords - v->coords;
    vec2 b = next->coords - v->coords;

    return (prev->coords.y >= v->coords.y && next->coords.y >= v->coords.y) && (det(a, b) > 0); 
}

bool isEndVertex(DCELVertex *v) {
    DCELVertex *prev = v->incidentEdge->prev->origin;
    DCELVertex *next = v->incidentEdge->twin->origin;

    vec2 a = prev->coords - v->coords;
    vec2 b = next->coords - v->coords;

    return (prev->coords.y >= v->coords.y && next->coords.y >= v->coords.y) && (det(a, b) <= 0); 
}

auto setCmp = [&dcel](DCELHalfEdge *a, DCELHalfEdge *b) {
    // TODO: Check if this is correct
    return std::min(dcel.origin(a)->coords.x, a->twin->origin->coords.x) < std::min(b->origin->coords.x, b->twin->origin->coords.x);
};

std::set<DCELHalfEdge *, decltype(setCmp)> t(setCmp);

std::vector<DCELVertex *> helper;

void handleVertex(DCEL *dcel, DCELVertex *v) {
    if (isStartVertex(v)) {
        t.insert(v->incidentEdge);
        helper[v->id] = v;
    } else if (isEndVertex(v)) {
        int eIminus1 = v->incidentEdge->prev->origin->id;

        if (isMergeVertex(helper[eIminus1])) {
            // TODO insert diagonal from v to helper[eIminus1]
            dcel->connect(v, helper[eIminus1]);
        }

        t.erase(v->incidentEdge->prev);
    } else if (isSplitVertex(v)) {
    } else if (isMergeVertex(v)) {
    } else { // Regular Vertex
    }
}

int main() {
    int n;

    std::cin >> n;

    std::vector<vec2> vertices;

    readVertices(n, vertices);

    DCEL dcel(vertices);

    auto pqCmp = [](DCELVertex *a, DCELVertex *b) {
        if (a->coords.y == b->coords.y) {
            return a->coords.x < b->coords.x;
        }
        return a->coords.y < b->coords.y;
    };

    std::priority_queue<DCELVertex*, std::vector<DCELVertex*>, decltype(pqCmp)> pq(pqCmp);
    
    helper.resize(n);

    DCELVertex* start, *curr;
    
    // Add vertices to queue
    start = curr = dcel.start();
    do {
        pq.push(curr);
        curr = curr->incidentEdge->twin->origin;
    } while (curr != start);

    while (!pq.empty()) {
        DCELVertex *v = pq.top();
        pq.pop();

        handleVertex(v);
    }

    return 0;
}
