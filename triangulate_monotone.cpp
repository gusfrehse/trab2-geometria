#include <vector>
#include <stack>

#include "dcel.hpp"



HalfEdgeId topMostVertex(DCEL &dcel, FaceId face)
{
    HalfEdgeId he = dcel.sampleEdge(face);
    bool flipped = false;

    if (dcel.coords(dcel.origin(he)).y < dcel.coords(dcel.origin(dcel.twin(he))).y)
    {
        flipped = true;
        he = dcel.twin(he);
    }

    while (dcel.coords(dcel.origin(dcel.twin(he))).y > dcel.coords(dcel.origin(he)).y)
    {
        he = dcel.next(he);
    }
    
    if (flipped)
        he = dcel.twin(he);

    return he;
}

std::vector<std::pair<HalfEdgeId, int>> mergeVertices(DCEL &dcel, FaceId face)
{
    std::vector<std::pair<HalfEdgeId, int>> u;
    HalfEdgeId he = topMostVertex(dcel, face);
    HalfEdgeId eh = he;
    
    // Merge vertices in decreasing y
    do
    {
        if (dcel.coords(dcel.origin(he)).y >= dcel.coords(dcel.origin(eh)).y)
        {
            u.push_back({he, 0});
            he = dcel.next(he);
        }
        else
        {
            u.push_back({eh, 1});
            eh = dcel.prev(eh);            
        }
    }
    while (he != eh);

    return u;
}

void triangulateMonotoneFace(DCEL &dcel, FaceId face)
{
    std::vector<std::pair<HalfEdgeId, int>> u = mergeVertices(dcel, face);
    
    std::stack<std::pair<HalfEdgeId, int>> s;
    
    s.push(u[0]);
    s.push(u[1]);
    
    for (int j = 2, n = u.size() - 1; j < n; j++)
    {
        if (u[j].second != s.top().second)
        {
            // Opposite sides

            while (s.size() > 1)
            {
                HalfEdgeId he = s.top().first;
                s.pop();
                
                dcel.connect(u[j].first, he);
                s.push(u[j - 1]);
                s.push(u[j]);
            }
        }
        else
        {
            // Same side
            s.pop();

            HalfEdgeId v = s.top().first;

            vec2 a = dcel.coords(u[j].first) - dcel.coords(v);
            vec2 e1 = dcel.coords(dcel.prev(v));
            vec2 e2 = dcel.coords(dcel.next(v));
            
            double cross1 = det(a, e1);
            double cross2 = det(e2, a);
            
            if (cross1 * cross2 > 0)
            {
                // Same sign

            }
        }
    }
}

void triangulateMonotone(DCEL &dcel)
{
    for (int i = 0, numFaces = dcel.faces.size(); i < numFaces; i++) {
        triangulateMonotoneFace(dcel, i);
    }
}