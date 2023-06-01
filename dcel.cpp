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

        halfEdges[2 * i + 1].origin = (i + 1) % CCWConvexVertices.size();
        halfEdges[2 * i + 1].twin = 2 * i;
        halfEdges[2 * i + 1].next = realMod(2 * (i - 1) + 1, 2 * CCWConvexVertices.size());
        halfEdges[2 * i + 1].prev = realMod(2 * (i + 1) + 1, 2 * CCWConvexVertices.size());
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
    // TODO: Fix this
    halfEdges.push_back({});
    halfEdges.push_back({});

    HalfEdgeId x = halfEdges.size() - 2;
    HalfEdgeId y = halfEdges.size() - 1;
    HalfEdgeId c = prev(a);
    HalfEdgeId d = prev(b);
    
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
}

std::ostream& operator<<(std::ostream& os, const DCELHalfEdge& halfEdge) {
    os << "HalfEdge { origin = " << halfEdge.origin << ", twin  = " << halfEdge.twin << ", next = " << halfEdge.next << ", prev = " << halfEdge.prev << " }";
    return os;
}