#include "triangulate_monotone.hpp"

#include <vector>
#include <stack>

#include "dcel.hpp"

HalfEdgeId topMostVertex(DCEL &dcel, FaceId face)
{
    std::cerr << "Finding topmost vertex..." << std::endl;
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


    std::cerr << "Topmost vertex is (" << dcel.coords(dcel.origin(he)) << ")" << std::endl;
    return he;
}

std::vector<std::pair<HalfEdgeId, int>> mergeVertices(DCEL &dcel, FaceId face)
{
    // TODO: this is wrong!
    std::cerr << "Merging vertices..." << std::endl;
    std::vector<std::pair<HalfEdgeId, int>> u;
    HalfEdgeId he = topMostVertex(dcel, face);
    HalfEdgeId eh = dcel.prev(he);
    
    
    HalfEdgeId current = he;
    do
    {
        std::cerr << current << " " << dcel.getHalfEdge(current) << " from " << dcel.origin(current) << " to (twin) " << dcel.origin(dcel.twin(current)) << " or (next) " << dcel.origin(dcel.next(current)) << std::endl;
        current = dcel.next(current);
    }
    while (current != he);

    exit(0);
    
    // Merge vertices in decreasing y
    do
    {
        std::cerr << "Analyzing " << dcel.coords(dcel.origin(he)) << " and " <<  dcel.coords(dcel.origin(he)) << std::endl;
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

    std::cerr << "Merged vertices" << std::endl;
    return u;
}

void triangulateMonotoneFace(DCEL &dcel, FaceId face)
{
    std::cerr << "Triangulating monotone face " << face << "..." << std::endl;
    std::vector<std::pair<HalfEdgeId, int>> u = mergeVertices(dcel, face);
    
    std::stack<std::pair<HalfEdgeId, int>> s;

    std::cerr << "U:\n";
    for (auto p : u)
    {
        std::cerr << p.first << " " << p.second << " (" << dcel.coords(dcel.origin(p.first)) << ")" << std::endl;
    }
    
    s.push(u[0]);
    s.push(u[1]);
    
    for (int j = 2, n = u.size() - 1; j < n; j++)
    {
        std::cerr << "Triangulating monotone face " << face << " edge " << u[j].first << "..." << std::endl;
        if (u[j].second != s.top().second)
        {
            std::cerr << "Opposite sides" << std::endl;
            // Opposite sides
            while (s.size() > 1)
            {
                HalfEdgeId he = s.top().first;
                s.pop();
                
                dcel.connect(u[j].first, he);
            }

            s.push(u[j - 1]);
            s.push(u[j]);
        }
        else
        {
            std::cerr << "Same side" << std::endl;
            // Same side
            s.pop();

            std::pair<HalfEdgeId, int> v = s.top();

            // TODO: Check if this is correct.
            vec2 a = dcel.coords(u[j].first) - dcel.coords(v.first);
            vec2 e1 = dcel.coords(dcel.prev(v.first));
            vec2 e2 = dcel.coords(dcel.next(v.first));
            
            double cross1 = det(a, e1);
            double cross2 = det(e2, a);
            
            while (cross1 * cross2 > 0)
            {
                // Same sign
                dcel.connect(u[j].first, v.first);
                v = s.top();
                s.pop();

                a = dcel.coords(u[j].first) - dcel.coords(v.first);
                e1 = dcel.coords(dcel.prev(v.first));
                e2 = dcel.coords(dcel.next(v.first));

                cross1 = det(a, e1);
                cross2 = det(e2, a);
            }

            s.push(v);
            s.push(u[j]);
        }
    }
}

void triangulateMonotone(DCEL &dcel)
{
    std::cerr << "Triangulating monotone faces..." << std::endl;
    for (int i = 0, numFaces = dcel.faces.size(); i < numFaces; i++) {
        triangulateMonotoneFace(dcel, i);
    }
}