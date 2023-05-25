#pragma once

#include "util.hpp"

#include <list>
#include <vector>

struct DCELHalfEdge;
struct DCELVertex;
struct DCELFace;

struct DCELVertex {
    vec2 coords;
    DCELHalfEdge *incidentEdge = nullptr;
};

struct DCELFace {
    DCELHalfEdge *outerComponent = nullptr;
};

struct DCELHalfEdge {
    DCELVertex *origin = nullptr;
    DCELHalfEdge *twin = nullptr;
    DCELFace *incidentFace = nullptr;
    
    DCELHalfEdge *next = nullptr, *prev = nullptr;
};

class DCEL {
    std::vector<DCELVertex> vertices;
    std::vector<DCELHalfEdge> halfEdges;
    std::vector<DCELFace> faces;

public:
    DCEL(std::vector<vec2> CCWConvexVertices);
    DCELVertex *start();
    DCELVertex *end();
};
