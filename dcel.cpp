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

bool isInsideTriangle(vec2 a, vec2 b, vec2 c, vec2 p) {
    vec2 v0 = c - a;
    vec2 v1 = b - a;
    vec2 v2 = p - a;

    double dot00 = dot(v0, v0);
    double dot01 = dot(v0, v1);
    double dot02 = dot(v0, v2);
    double dot11 = dot(v1, v1);
    double dot12 = dot(v1, v2);

    double den = (dot00 * dot11 - dot01 * dot01);

    double u = (dot11 * dot02 - dot01 * dot12) / den;
    double v = (dot00 * dot12 - dot01 * dot02) / den;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool DCEL::isEar(HalfEdgeId id)
{
    if (next(next(id)) == id)
        return false; 

    vec2 currCoords = coords(origin(id));
    vec2 nextCoords = coords(origin(next(id)));
    vec2 prevCoords = coords(origin(prev(id)));

    double detVal = det(prevCoords - currCoords, nextCoords - currCoords);

    if (detVal > 0)
        return false;
    
    HalfEdgeId start = next(id);
    HalfEdgeId current = start;
    while (prev(start) != current)
    {
        if (isInsideTriangle(currCoords, nextCoords, prevCoords, coords(origin(current))))
        {
            return false;
        }

        current = next(current);
    }
    
    
    return true;
}

void DCEL::print()
{
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
