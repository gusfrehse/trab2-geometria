#include <iostream>
#include <vector>
#include <array>
#include <deque>
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

void createPolygonDeque(int n, std::deque<edge>& polygon) {
    polygon.resize(n);

    for (int i = 0; i < n; ++i) {
        polygon[i] = { i, (i + 1) % n };
    }
}

std::vector<vec2> vertices; // vertices of the polygon
std::deque<edge> polygon; // edges of the polygon 

std::pair<int, int> getNeighbours(int i) {
    return { (i - 1 + vertices.size()) % vertices.size(), (i + 1) % vertices.size()};
}

float angle(int i) {
    auto [prev, next] = getNeighbours(i);

    vec2 a = vertices[prev] - vertices[i];
    vec2 b = vertices[next] - vertices[i];

    return std::acos(dot(a, b) / (a.length() * b.length()));
}

void handleSplitVertex(int v, std::set<std::pair<edge, int>>& t) {
    t.insert({})
}

void handleStartVertex(int v, std::set<std::pair<edge, int>>& t) {
}

void handleMergeVertex(int v, std::set<std::pair<edge, int>>& t) {
}

void handleEndVertex(int v, std::set<std::pair<edge, int>>& t) {
}

bool isStartVertex(int v) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    if ((vertices[prev].y <= vertices[v].y && vertices[next].y <= vertices[v].y) && (det(a, b) > 0))
        return true;

    return false;
}

void handleVertex(int v, std::set<std::pair<edge, int>>& t) {
    auto [prev, next] = getNeighbours(v);
    vec2 a = vertices[prev] - vertices[v];
    vec2 b = vertices[next] - vertices[v];

    if (vertices[prev].y <= vertices[v].y && vertices[next].y <= vertices[v].y) {
        if (det(a, b) > 0) {
            // split vertex
            handleSplitVertex(v, t);
            //std::cout << "v = " << v << " = (" << vertices[v].x << ", " << vertices[v].y << ") split vertex!" << std::endl;
        } else {
            // start vertex
            handleStartVertex(v, t);
            //std::cout << "v = " << v << " = (" << vertices[v].x << ", " << vertices[v].y << ") start vertex!" << std::endl;
        }
    } else if (vertices[prev].y >= vertices[v].y && vertices[next].y >= vertices[v].y) {
        if (det(a, b) > 0) {
            // merge vertex
            handleMergeVertex(v, t);
            //std::cout << "v = " << v << " = (" << vertices[v].x << ", " << vertices[v].y << ") merge vertex!" << std::endl;
        } else {
            // end vertex
            handleEndVertex(v, t);
            //std::cout << "v = " << v << " = (" << vertices[v].x << ", " << vertices[v].y << ") end vertex!" << std::endl;
        }
    } else {
        // regular vertex
        //std::cout << "v = " << v << " = (" << vertices[v].x << ", " << vertices[v].y << ") regular vertex!" << std::endl;
    }
}

int main() {
    int n;

    std::cin >> n;

    readVertices(n, vertices);

    createPolygonDeque(n, polygon);

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

    std::set<std::pair<edge, int>> t;

    while (!pq.empty()) {
        int v = pq.top();
        pq.pop();

        handleVertex(v, t);
    }

    return 0;
}
