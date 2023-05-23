#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>

#include "util.hpp"

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

void handleSplitVertex(int v, std::set<edge>& t, std::vector<int>& helper) {
    // TODO
}

void handleStartVertex(int v, std::set<edge>& t, std::vector<int>& helper) {
}

void handleMergeVertex(int v, std::set<edge>& t, std::vector<int>& helper) {
    // TODO
}

void handleEndVertex(int v, std::set<edge>& t, std::vector<int>& helper) {
    if (isMergeVertex(helper[(v - 1) % vertices.size()])) {
        // TODO
    }
}

bool isSplitVertex(int v) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    return (vertices[prev].y <= vertices[v].y && vertices[next].y <= vertices[v].y) && (det(a, b) > 0); 
}

bool isStartVertex(int v) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    return (vertices[prev].y <= vertices[v].y && vertices[next].y <= vertices[v].y) && (det(a, b) <= 0); 
}

bool isMergeVertex(int v) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    return (vertices[prev].y >= vertices[v].y && vertices[next].y >= vertices[v].y) && (det(a, b) > 0);
}

bool isEndVertex(int v) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    return (vertices[prev].y >= vertices[v].y && vertices[next].y >= vertices[v].y) && (det(a, b) <= 0);
}

edge getEdge(int v) {
    return { v, (v + 1) % vertices.size() };
}

void handleVertex(int v, std::set<edge>& t, std::vector<int>& helper) {
    if (isStartVertex(v)) {

        t.insert(getEdge(v));
        helper[v] = v;

    } else if (isEndVertex(v)) {

        if (isMergeVertex(helper[(v - 1) % vertices.size()])) {
            // Insert diagonal in polygon
            auto edge = { v, helper[(v - 1) % vertices.size()] };

            for (auto it = polygon.begin(); it != polygon.end(); ++it) {
                if (it->first == v) {
                }
            }
        }
        
        t.erase(getEdge(v));
        
    } else if (isSplitVertex(v)) {

    } else if (isMergeVertex(v)) {
    } else { // Regular Vertex
    }
}

int main() {
    int n;

    std::cin >> n;

    readVertices(n, vertices);

    createPolygonList(n, polygon);

    for (const auto& e : polygon) {
        std::cout << e.first << " " << e.second << std::endl;
    }

    auto pqCmp = [](int a, int b) {
        if (vertices[a].y == vertices[b].y) {
            return vertices[a].x < vertices[b].x;
        }

        return vertices[a].y < vertices[b].y;
    };

    std::priority_queue<int, std::vector<int>, decltype(pqCmp)> pq(pqCmp);

    for (int i = 0; i < n; ++i) {
        pq.push(i);
    }

    auto setCmp = [](const std::pair<edge, int>& a, const std::pair<edge, int>& b) {
        // TODO: Check if this is correct
        return std::min(vertices[a.first.first].x, vertices[a.first.second].x) < std::min(vertices[b.first.first].x, vertices[b.first.second].x);
    };

    std::set<edge> t;

    while (!pq.empty()) {
        int v = pq.top();
        pq.pop();

        handleVertex(v, t);
    }

    return 0;
}
