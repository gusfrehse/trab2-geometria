#include "dcel.hpp"

#include "util.hpp"

#include <vector>

int realMod(int a, int b) {
    return (a % b + b) % b;
}

DCEL::DCEL(std::vector<vec2> CCWConvexVertices) {
    vertices.resize(CCWConvexVertices.size());
    halfEdges.resize(2 * CCWConvexVertices.size());
    
    for (int i = 0; i < CCWConvexVertices.size(); ++i) {
        vertices[i].coords = CCWConvexVertices[i];
        vertices[i].incidentEdge = 2 * i;
        vertices[i].id = i;

        halfEdges[2 * i].origin = i;
        halfEdges[2 * i].twin = 2 * i + 1;
        halfEdges[2 * i].next = realMod(2 * (i + 1), 2 * CCWConvexVertices.size());
        halfEdges[2 * i].prev = realMod(2 * (i - 1), 2 * CCWConvexVertices.size());

        halfEdges[2 * i + 1].origin = i + 1;
        halfEdges[2 * i + 1].twin = 2 * i;
        halfEdges[2 * i + 1].next = realMod(2 * (i - 1), 2 * CCWConvexVertices.size());
        halfEdges[2 * i + 1].prev = realMod(2 * (i + 1), 2 * CCWConvexVertices.size());
    }
}

HalfEdgeId DCEL::start() {
    return 0;
}

// TODO: this is wrong shouldn't be used. (use start() instead and check if it's the same vertex)
HalfEdgeId DCEL::end() {
    return halfEdges.size() - 1;
}

DCELVertex& DCEL::getVertex(VertexId id)
{
    return vertices[id];
}

DCELHalfEdge& DCEL::getHalfEdge(HalfEdgeId id)
{
    return halfEdges[id];
}

void DCEL::connect(HalfEdgeId a, HalfEdgeId b) {
    halfEdges.push_back({});
    halfEdges.push_back({});

    HalfEdgeId newA = halfEdges.size() - 2;
    HalfEdgeId newB = halfEdges.size() - 1;

    prev(newA) = prev(b);
    prev(newB) = prev(a);

    next(newA) = a;
    next(newB) = b;

    next(prev(a)) = newB;
    next(prev(b)) = newA;

    prev(a) = newA;
    prev(b) = newB;
}