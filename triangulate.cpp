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

bool isSplitVertex(DCEL& dcel, VertexId v) {
    VertexId prev = dcel.origin(dcel.prev(dcel.incidentEdge(v)));
    VertexId next = dcel.origin(dcel.twin(dcel.incidentEdge(v)));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(v).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y <= vCoords.y && nextCoords.y <= vCoords.y) && (det(a, b) > 0); 
}

bool isStartVertex(DCEL& dcel, VertexId v) {
    VertexId prev = dcel.origin(dcel.prev(dcel.incidentEdge(v)));
    VertexId next = dcel.origin(dcel.twin(dcel.incidentEdge(v)));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(v).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y <= vCoords.y && nextCoords.y <= vCoords.y) && (det(a, b) <= 0); 
}

bool isMergeVertex(DCEL& dcel, VertexId v) {
    VertexId prev = dcel.origin(dcel.prev(dcel.incidentEdge(v)));
    VertexId next = dcel.origin(dcel.twin(dcel.incidentEdge(v)));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(v).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y >= vCoords.y && nextCoords.y >= vCoords.y) && (det(a, b) > 0); 
}

bool isEndVertex(DCEL& dcel, VertexId v) {
    VertexId prev = dcel.origin(dcel.prev(dcel.incidentEdge(v)));
    VertexId next = dcel.origin(dcel.twin(dcel.incidentEdge(v)));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(v).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y >= vCoords.y && nextCoords.y >= vCoords.y) && (det(a, b) <= 0); 
}

std::vector<VertexId> helper;
std::set<HalfEdgeId> t;

void handleVertex(DCEL &dcel, VertexId v) {
    if (isStartVertex(dcel, v)) {
        t.insert(dcel.incidentEdge(v));
        helper[v] = v;
    } else if (isEndVertex(dcel, v)) {
        int eIminus1 = dcel.origin(dcel.prev(dcel.incidentEdge(v)));

        if (isMergeVertex(dcel, helper[eIminus1])) {
            // TODO insert diagonal from v to helper[eIminus1]
            dcel.connect(v, helper[eIminus1]);
        }

        t.erase(dcel.prev(dcel.incidentEdge(v)));
    } else if (isSplitVertex(dcel, v)) {
    } else if (isMergeVertex(dcel, v)) {
    } else { // Regular Vertex
    }
}

int main() {
    int n;

    std::cin >> n;

    std::vector<vec2> vertices;

    readVertices(n, vertices);

    DCEL dcel(vertices);

    auto pqCmp = [&dcel](VertexId a, VertexId b) {
        vec2 aCoords = dcel.getVertex(a).coords;
        vec2 bCoords = dcel.getVertex(b).coords;

        if (aCoords.y == bCoords.y) {
            return aCoords.x < bCoords.x;
        }
        return aCoords.y < bCoords.y;
    };

    std::priority_queue<VertexId, std::vector<VertexId>, decltype(pqCmp)> pq(pqCmp);
    
    helper.resize(n);

    VertexId start, curr;
    
    // Add vertices to queue
    start = curr = dcel.start();
    do {
        pq.push(curr);
        curr = dcel.origin(dcel.twin(dcel.incidentEdge(curr)));
    } while (curr != start);

    while (!pq.empty()) {
        VertexId v = pq.top();
        pq.pop();

        handleVertex(dcel, v);
    }

    return 0;
}
