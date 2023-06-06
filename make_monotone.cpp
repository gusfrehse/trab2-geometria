#include "make_monotone.hpp"
#include "dcel.hpp"
#include "util.hpp"

#include <set>
#include <queue>

bool isSplitVertex(DCEL& dcel, HalfEdgeId v) {
    VertexId prev = dcel.origin(dcel.prev(v));
    VertexId next = dcel.origin(dcel.twin(v));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(dcel.origin(v)).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y <= vCoords.y && nextCoords.y <= vCoords.y) && (det(a, b) > 0); 
}

bool isStartVertex(DCEL& dcel, HalfEdgeId v) {
    VertexId prev = dcel.origin(dcel.prev(v));
    VertexId next = dcel.origin(dcel.twin(v));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(dcel.origin(v)).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y <= vCoords.y && nextCoords.y <= vCoords.y) && (det(a, b) <= 0); 
}

bool isMergeVertex(DCEL& dcel, HalfEdgeId v) {
    VertexId prev = dcel.origin(dcel.prev(v));
    VertexId next = dcel.origin(dcel.twin(v));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(dcel.origin(v)).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y >= vCoords.y && nextCoords.y >= vCoords.y) && (det(a, b) > 0); 
}

bool isEndVertex(DCEL& dcel, HalfEdgeId v) {
    VertexId prev = dcel.origin(dcel.prev(v));
    VertexId next = dcel.origin(dcel.twin(v));

    vec2 prevCoords = dcel.getVertex(prev).coords;
    vec2 nextCoords = dcel.getVertex(next).coords;
    vec2 vCoords = dcel.getVertex(dcel.origin(v)).coords;

    vec2 a = prevCoords - vCoords;
    vec2 b = nextCoords - vCoords;

    return (prevCoords.y >= vCoords.y && nextCoords.y >= vCoords.y) && (det(a, b) <= 0); 
}

bool liesToTheRightOfThePolygon(DCEL& dcel, HalfEdgeId he) {
    vec2 vCoords = dcel.getVertex(dcel.origin(he)).coords;
    vec2 nextCoords = dcel.getVertex(dcel.origin(dcel.next(he))).coords;

    // check if next vertex is below v (only works if polygon vertices are in counter clockwise order)
    return nextCoords.y > vCoords.y;   
}

DCEL dcel;
DCEL *dcelPtr = &dcel;

auto toTheLeft = [](const HalfEdgeId &a, const HalfEdgeId &b) {
    vec2 aCoords = dcelPtr->getVertex(dcelPtr->origin(a)).coords;
    vec2 bCoords = dcelPtr->getVertex(dcelPtr->origin(b)).coords;
    vec2 bOpCoords = dcelPtr->getVertex(dcelPtr->origin(dcel.twin(b))).coords;

    return det(aCoords - bCoords, bOpCoords - bCoords) > 0;
};

std::vector<HalfEdgeId> helper;
std::set<HalfEdgeId, decltype(toTheLeft)> t;

void handleVertex(DCEL &dcel, HalfEdgeId v)
{
    std::cerr << "Handling vertex " << dcel.origin(v) << " he " << v << std::endl;
    if (isStartVertex(dcel, v))
    {
        std::cerr << "\tis start vertex" << std::endl;
        t.insert(v);
        helper[v] = v;
    }
    else if (isEndVertex(dcel, v))
    {
        std::cerr << "\tis end vertex" << std::endl;
        HalfEdgeId eIminus1 = dcel.prev(v);

        std::cerr << "\te i-1 is " << dcel.origin(v) << std::endl;

        if (isMergeVertex(dcel, helper[eIminus1]))
        {
            std::cerr << "\thelper[eIminus1] (he: " << helper[eIminus1] <<  ", vertex: " << dcel.origin(helper[eIminus1]) <<  ") is merge vertex" << std::endl;
            dcel.connect(v, helper[eIminus1]);
        } else {
            std::cerr << "\thelper[eIminus1] (he: " << helper[eIminus1] <<  ", vertex: " << dcel.origin(helper[eIminus1]) <<  ") is not merge vertex" << std::endl;
        }

        t.erase(eIminus1);
    }
    else if (isSplitVertex(dcel, v))
    {
        std::cerr << "\tis split vertex" << std::endl;
        HalfEdgeId ej = *t.lower_bound(v);
        
        std::cerr << "\tedge directly left of v " << dcel.getHalfEdge(ej) << std::endl;
        std::cerr << "\thelper[ej]" << dcel.getHalfEdge(helper[ej]) << std::endl;

        std::cerr << "\tconnecting " << dcel.getHalfEdge(v) << " and " << dcel.getHalfEdge(helper[ej]) << std::endl;
        dcel.connect(v, helper[ej]);

        helper[ej] = v;

        t.insert(v);
        helper[v] = v;
    }
    else if (isMergeVertex(dcel, v))
    {
        std::cerr << "\tis merge vertex" << std::endl;
        HalfEdgeId eIminus1 = dcel.prev(v);

        if (isMergeVertex(dcel, helper[eIminus1]))
        {
            dcel.connect(v, helper[eIminus1]);
        }

        t.erase(eIminus1);

        HalfEdgeId ej = *t.lower_bound(v);

        if (isMergeVertex(dcel, helper[ej]))
        {
            dcel.connect(v, helper[ej]);
        }

        helper[ej] = v;
    }
    else
    {
        // Regular Vertex
        std::cerr << "\tis regular vertex" << std::endl;
        HalfEdgeId eIminus1 = dcel.prev(v);

        if (liesToTheRightOfThePolygon(dcel, v))
        {
            std::cerr << "\tv lies to the right of the polygon" << std::endl;
            std::cerr << "\teIminus1 " << dcel.getHalfEdge(eIminus1) << std::endl;
            std::cerr << "\thelper[eIminus1] " << dcel.getHalfEdge(helper[eIminus1]) << std::endl;
            if (isMergeVertex(dcel, helper[eIminus1])) {
                std::cerr << "\thelper[eIminus1] is merge vertex" << std::endl;
                dcel.connect(v, helper[eIminus1]);
            }
            else
            {
                std::cerr << "\thelper[eIminus1] is not merge vertex" << std::endl;
            }

            t.erase(eIminus1);
            
            t.insert(v);
            helper[v] = v;
        }
        else
        {
            std::cerr << "\tv lies to the left of the polygon" << std::endl;
            HalfEdgeId ej = *t.lower_bound(v);
            std::cerr << "\tej " << dcel.getHalfEdge(ej) << std::endl;
            std::cerr << "\thelper[ej] " << dcel.getHalfEdge(helper[ej]) << std::endl;
            
            if (isMergeVertex(dcel, helper[ej]))
            {
                std::cerr << "\thelper[ej] is merge vertex" << std::endl;
                dcel.connect(v, helper[ej]);
            }
            else
            {
                std::cerr << "\thelper[ej] is not merge vertex" << std::endl;
            }
            
            helper[ej] = v;
        }
    }
}

DCEL makeMonotone(std::vector<vec2> vertices) {
    dcel.insertVertices(vertices);
    
    auto pqCmp = [](HalfEdgeId a, HalfEdgeId b) {
        vec2 aCoords = dcel.getVertex(dcel.origin(a)).coords;
        vec2 bCoords = dcel.getVertex(dcel.origin(b)).coords;

        if (aCoords.y == bCoords.y) {
            return aCoords.x < bCoords.x;
        }
        return aCoords.y < bCoords.y;
    };

    std::priority_queue<HalfEdgeId, std::vector<HalfEdgeId>, decltype(pqCmp)> pq(pqCmp);
    
    helper.resize(2 * vertices.size());

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
    
    return dcel;
}