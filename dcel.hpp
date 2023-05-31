#pragma once

#include "util.hpp"

#include <list>
#include <vector>

struct DCELHalfEdge;
struct DCELVertex;

using HalfEdgeId = int;
using VertexId = int;

struct DCELVertex {
    vec2 coords;
    int id;
    HalfEdgeId incidentEdge = -1;
};

struct DCELHalfEdge {
    VertexId origin = -1;
    HalfEdgeId twin = -1;
    
    HalfEdgeId next = -1, prev = -1;
};

class DCEL {
    std::vector<DCELVertex> vertices;
    std::vector<DCELHalfEdge> halfEdges;

public:
    DCEL(std::vector<vec2> CCWConvexVertices);
    VertexId start();
    VertexId end();

    DCELVertex& getVertex(VertexId id);
    DCELHalfEdge& getHalfEdge(HalfEdgeId id);

    HalfEdgeId& incidentEdge(VertexId id) { return vertices[id].incidentEdge; };
    HalfEdgeId& next(HalfEdgeId id) { return halfEdges[id].next; }
    HalfEdgeId& prev(HalfEdgeId id) { return halfEdges[id].prev;}
    HalfEdgeId& twin(HalfEdgeId id) { return halfEdges[id].twin;}
    VertexId origin(HalfEdgeId id) { return halfEdges[id].origin;}

    void connect(HalfEdgeId a, HalfEdgeId b);
};
