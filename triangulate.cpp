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

void createPolygonList(int n, std::list<edge>& polygon) {
    for (int i = 0; i < n; ++i) {
        polygon.push_back({ i, (i + 1) % n });
    }
}

std::vector<vec2> vertices; // vertices of the polygon
std::list<edge> polygon; // edges of the polygon 
std::vector<int> helper; // helper array

std::pair<int, int> getNeighbours(int i) {
    return { (i - 1 + vertices.size()) % vertices.size(), (i + 1) % vertices.size()};
}

float angle(int i) {
    auto [prev, next] = getNeighbours(i);

    vec2 a = vertices[prev] - vertices[i];
    vec2 b = vertices[next] - vertices[i];

    return std::acos(dot(a, b) / (a.length() * b.length()));
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

edge getEdge(int v) {
    return { v, (v + 1) % vertices.size() };
}

void handleVertex(DCELVertex *v, std::vector<DCELVertex *>& helper) {
    if (isStartVertex(v)) {
    } else if (isEndVertex(v)) {
    } else if (isSplitVertex(v)) {

    } else if (isMergeVertex(v)) {
    } else { // Regular Vertex
    }
}

int main() {
    int n;

    std::cin >> n;

    readVertices(n, vertices);

    DCEL dcel(vertices);

    createPolygonList(n, polygon);

    for (const auto& e : polygon) {
        std::cout << e.first << " " << e.second << std::endl;
    }

    auto pqCmp = [](DCELVertex *a, DCELVertex *b) {
        if (a->coords.y == b->coords.y) {
            return a->coords.x < b->coords.x;
        }

        return a->coords.y < b->coords.y;
    };

    std::priority_queue<DCELVertex*, std::vector<DCELVertex*>, decltype(pqCmp)> pq(pqCmp);

    DCELVertex* start, *curr;
    start = curr = dcel.start();
    do {
        pq.push(curr);
        curr = curr->incidentEdge->twin->origin;
    } while (curr != start);

    auto setCmp = [](const std::pair<edge, int>& a, const std::pair<edge, int>& b) {
        // TODO: Check if this is correct
        return std::min(vertices[a.first.first].x, vertices[a.first.second].x) < std::min(vertices[b.first.first].x, vertices[b.first.second].x);
    };

    std::set<edge> t;

    std::vector<DCELVertex *> helper(n);

    while (!pq.empty()) {
        DCELVertex *v = pq.top();
        pq.pop();

        handleVertex(v, helper);
    }

    return 0;
}
