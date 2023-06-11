#include "make_monotone.hpp"
#include "dcel.hpp"
#include "util.hpp"
#include "tree.hpp"

#include <set>
#include <queue>
#include <gtest/gtest.h>

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
std::vector<HalfEdgeId> helper;
Tree t(dcel);

void handleVertex(DCEL &dcel, HalfEdgeId v)
{
    if (isStartVertex(dcel, v))
    {
        // Start Vertex
        t.insert(v);
        helper[v] = v;
    }
    else if (isEndVertex(dcel, v))
    {
        // End Vertex
        HalfEdgeId eIminus1 = dcel.prev(v);

        if (isMergeVertex(dcel, helper[eIminus1]))
        {
            dcel.connect(v, helper[eIminus1]);
        }

        t.remove(eIminus1);
    }
    else if (isSplitVertex(dcel, v))
    {
        // Split Vertex
        HalfEdgeId ej = t.get(dcel.getVertex(dcel.origin(v)).coords);

        dcel.connect(v, helper[ej]);

        helper[ej] = v;

        t.insert(v);
        helper[v] = v;
    }
    else if (isMergeVertex(dcel, v))
    {
        // Merge Vertex
        HalfEdgeId eIminus1 = dcel.prev(v);

        if (isMergeVertex(dcel, helper[eIminus1]))
        {
            dcel.connect(v, helper[eIminus1]);
        }

        t.remove(eIminus1);

        HalfEdgeId ej = t.get(dcel.getVertex(dcel.origin(v)).coords);

        if (isMergeVertex(dcel, helper[ej]))
        {
            dcel.connect(v, helper[ej]);
        }

        helper[ej] = v;
    }
    else
    {
        // Regular Vertex
        HalfEdgeId eIminus1 = dcel.prev(v);

        if (!liesToTheRightOfThePolygon(dcel, v))
        {
            if (isMergeVertex(dcel, helper[eIminus1])) {
                dcel.connect(v, helper[eIminus1]);
            }

            t.remove(eIminus1);
            
            t.insert(v);
            helper[v] = v;
        }
        else
        {
            HalfEdgeId ej = t.get(dcel.getVertex(dcel.origin(v)).coords);
            
            if (isMergeVertex(dcel, helper[ej]))
            {
                dcel.connect(v, helper[ej]);
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
    
    helper.resize(2 * vertices.size() + 1);

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
