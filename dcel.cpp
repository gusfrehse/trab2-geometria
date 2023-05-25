#include "dcel.hpp"

#include "util.hpp"

#include <vector>

int realMod(int a, int b) {
    return (a % b + b) % b;
}

DCEL::DCEL(std::vector<vec2> CCWConvexVertices) {
    vertices.resize(CCWConvexVertices.size());
    halfEdges.resize(2 * CCWConvexVertices.size());
    faces.resize(1);
    
    for (int i = 0; i < CCWConvexVertices.size(); ++i) {
        vertices[i].coords = CCWConvexVertices[i];
        vertices[i].incidentEdge = &halfEdges[2 * i];

        halfEdges[2 * i].origin = &vertices[i];
        halfEdges[2 * i].twin = &halfEdges[2 * i + 1];
        halfEdges[2 * i].next = &halfEdges[realMod(2 * (i + 1), 2 * CCWConvexVertices.size())];
        halfEdges[2 * i].prev = &halfEdges[realMod(2 * (i - 1), 2 * CCWConvexVertices.size())];
        halfEdges[2 * i].incidentFace = &faces[0];

        halfEdges[2 * i + 1].origin = &vertices[i + 1];
        halfEdges[2 * i + 1].twin = &halfEdges[2 * i];
        halfEdges[2 * i + 1].next = &halfEdges[realMod(2 * (i - 1), 2 * CCWConvexVertices.size())];
        halfEdges[2 * i + 1].prev = &halfEdges[realMod(2 * (i + 1), 2 * CCWConvexVertices.size())];
        halfEdges[2 * i + 1].incidentFace = &faces[0]; // TODO: Check if correct maybe another "outer face" is better
    }
    
    faces[0].outerComponent = &halfEdges[0];
}

DCELVertex *DCEL::start() {
    if (vertices.size() == 0) {
        return nullptr;
    }

    return &vertices[0];
}

DCELVertex *DCEL::end() {
    if (vertices.size() == 0) {
        return nullptr;
    }

    return &vertices[vertices.size() - 1];
}