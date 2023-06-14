#include "dcel.hpp"

#include "util.hpp"

#include <vector>

#include <cassert>

int realMod(int a, int b) {
    return (a % b + b) % b;
}

DCEL::DCEL(std::vector<vec2> CCWConvexVertices) {
    insertVertices(CCWConvexVertices);
}

void DCEL::insertVertices(std::vector<vec2> CCWConvexVertices) {
    vertices.resize(CCWConvexVertices.size());
    halfEdges.resize(2 * CCWConvexVertices.size());
    faces.push_back({0});
    
    for (long unsigned int i = 0; i < CCWConvexVertices.size(); ++i) {
        vertices[i].coords = CCWConvexVertices[i];
        vertices[i].incidentEdge = 2 * i;
        vertices[i].id = i;

        halfEdges[2 * i].origin = i;
        halfEdges[2 * i].twin = 2 * i + 1;
        halfEdges[2 * i].next = realMod(2 * (i + 1), 2 * CCWConvexVertices.size());
        halfEdges[2 * i].prev = realMod(2 * (i - 1), 2 * CCWConvexVertices.size());

        halfEdges[2 * i + 1].origin = (i + 1) % CCWConvexVertices.size();
        halfEdges[2 * i + 1].twin = 2 * i;
        halfEdges[2 * i + 1].next = realMod(2 * (i - 1) + 1, 2 * CCWConvexVertices.size());
        halfEdges[2 * i + 1].prev = realMod(2 * (i + 1) + 1, 2 * CCWConvexVertices.size());
    }
}

HalfEdgeId DCEL::start() {
    return 0;
}

DCELVertex& DCEL::getVertex(VertexId id)
{
    return vertices[id];
}

DCELHalfEdge& DCEL::getHalfEdge(HalfEdgeId id)
{
    return halfEdges[id];
}


/*          c
    * <--------------- *
    | \\               ^
    |   \\             |
    |     \\           |
   a|      x\\y       b|
    |         \\       |
    |           \\     |
    |             \\   |
    v               \\ |
    * ---------------> * 
            d
*/
void DCEL::connect(HalfEdgeId a, HalfEdgeId b) {
    halfEdges.push_back({});
    halfEdges.push_back({});

    HalfEdgeId x = halfEdges.size() - 2;
    HalfEdgeId y = halfEdges.size() - 1;
    HalfEdgeId c = prev(a);
    HalfEdgeId d = prev(b);

    faces.push_back({y}); 
    
    next(x) = a;
    next(y) = b;
    
    prev(x) = d;
    prev(y) = c;

    next(d) = x;
    next(c) = y;

    prev(a) = x;
    prev(b) = y;
    
    twin(x) = y;
    twin(y) = x;

    origin(x) = origin(b);
    origin(y) = origin(a);
    
    getVertex(origin(x)).incidentEdge = x;
    getVertex(origin(y)).incidentEdge = y;
}

void DCEL::print() {
    std::cout << vertices.size() << " " << halfEdges.size() / 2 <<  "\n";

    // print vertices
    for (const auto& v : vertices) {
        std::cout << v.coords << "\n";
    }

    // print edges
    for (long unsigned int i = 0; i < halfEdges.size(); ++++i) {
        std::cout << halfEdges[i].origin << " " << origin(next(i)) << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const DCELHalfEdge& halfEdge) {
    os << "HalfEdge { origin = " << halfEdge.origin << ", twin  = " << halfEdge.twin << ", next = " << halfEdge.next << ", prev = " << halfEdge.prev << " }";
    return os;
}

double DCEL::getXfromY(HalfEdgeId he, double y) {
    vec2 a = getVertex(origin(he)).coords;
    vec2 b = getVertex(origin(twin(he))).coords;

    // y - a.y = ((b.y - a.y) / (b.x - a.x)) * (x - a.x) <=> x = (y - a.y) * ((b.x - a.x) / (b.y - a.y)) + a.x
    return (y - a.y) * ((b.x - a.x) / (b.y - a.y)) + a.x;
}
