#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>

#include "util.hpp"

#include "dcel.hpp"

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

DCEL dcel;

auto toTheLeft = [](const HalfEdgeId &a, const HalfEdgeId &b) {
    vec2 aCoords = dcel.getVertex(dcel.origin(a)).coords;
    vec2 bCoords = dcel.getVertex(dcel.origin(b)).coords;
    vec2 bOpCoords = dcel.getVertex(dcel.origin(dcel.twin(b))).coords;

    return det(aCoords - bCoords, bOpCoords - bCoords) > 0;
};

std::vector<VertexId> helper;
std::set<HalfEdgeId, decltype(toTheLeft)> t;

void handleVertex(DCEL &dcel, HalfEdgeId v) {
    if (isStartVertex(dcel, dcel.origin(v))) {
        t.insert(dcel.incidentEdge(v));
        helper[v] = v;
    } else if (isEndVertex(dcel, dcel.origin(v))) {
        int eIminus1 = dcel.origin(dcel.prev(dcel.incidentEdge(v)));

        if (isMergeVertex(dcel, helper[eIminus1])) {
            // TODO insert diagonal from v to helper[eIminus1]
            dcel.connect(v, helper[eIminus1]);
        }

        t.erase(dcel.prev(dcel.incidentEdge(v)));
    } else if (isSplitVertex(dcel, dcel.origin(v))) {
    } else if (isMergeVertex(dcel, dcel.origin(v))) {
    } else { // Regular Vertex
    }
}

int main() {
    int n;

    std::cin >> n;

    std::vector<vec2> vertices;

    readVertices(n, vertices);

    DCEL dcel(vertices);

    auto pqCmp = [&dcel](HalfEdgeId a, HalfEdgeId b) {
        vec2 aCoords = dcel.getVertex(dcel.origin(a)).coords;
        vec2 bCoords = dcel.getVertex(dcel.origin(b)).coords;

        if (aCoords.y == bCoords.y) {
            return aCoords.x < bCoords.x;
        }
        return aCoords.y < bCoords.y;
    };

    std::priority_queue<HalfEdgeId, std::vector<HalfEdgeId>, decltype(pqCmp)> pq(pqCmp);
    
    helper.resize(n);

    VertexId start, curr;
    
    // Add vertices (which are identified by a edge which they are the origin to) to queue
    start = curr = dcel.start();
    do {
        pq.push(curr);
        curr = dcel.next(curr);
    } while (curr != start);

    while (!pq.empty()) {
        HalfEdgeId v = pq.top();
        pq.pop();

        handleVertex(dcel, v);
    }

    return 0;
}
