#pragma once

#include "util.hpp"

#include <list>
#include <vector>

struct DCELHalfEdge;
struct DCELVertex;

using HalfEdgeId = int;
using VertexId = int;
using FaceId = int;

struct DCELVertex {
    vec2 coords;
    HalfEdgeId incidentEdge = -1;
};

struct DCELHalfEdge {
    VertexId origin = -1;
    HalfEdgeId twin = -1;
    
    HalfEdgeId next = -1, prev = -1;

    FaceId incidentFace = -1;

    bool ear = false;

    friend std::ostream& operator<<(std::ostream& os, const DCELHalfEdge& halfEdge);
};

struct DCELFace {
    HalfEdgeId sampleEdge = -1;
};

class DCEL {
    public:
    std::vector<DCELVertex> vertices;
    std::vector<DCELHalfEdge> halfEdges;
    std::vector<DCELFace> faces;

    void insertVertices(std::vector<vec2> CCWConvexVertices);
    double getXfromY(HalfEdgeId a, double y);

public:
    DCEL(std::vector<vec2> CCWConvexVertices);
    DCEL() = default;

    HalfEdgeId start();

    void setupEars();

    DCELVertex& getVertex(VertexId id);
    DCELHalfEdge& getHalfEdge(HalfEdgeId id);

    FaceId& incidentFace(HalfEdgeId id) { return halfEdges[id].incidentFace; }
    HalfEdgeId& incidentEdge(VertexId id) { return vertices[id].incidentEdge; }
    HalfEdgeId& next(HalfEdgeId id) { return halfEdges[id].next; }
    HalfEdgeId& prev(HalfEdgeId id) { return halfEdges[id].prev; }
    HalfEdgeId& twin(HalfEdgeId id) { return halfEdges[id].twin; }
    VertexId& origin(HalfEdgeId id) { return halfEdges[id].origin; }
    HalfEdgeId& sampleEdge(FaceId id) { return faces[id].sampleEdge; } 
    vec2& coords(VertexId id) { return vertices[id].coords; }
    bool& ear(HalfEdgeId id) { return halfEdges[id].ear; }
    bool isEar(HalfEdgeId id);

    void print();

    FaceId connect(HalfEdgeId a, HalfEdgeId b);
};
