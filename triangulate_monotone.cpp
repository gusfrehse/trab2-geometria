
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

void triangulateMonotoneFace(DCEL &dcel, FaceId face)
{
    HalfEdgeId he = topMostVertex(dcel, face);
    HalfEdgeId eh = dcel.next(dcel.twin(he));
}


void triangulateMonotone(DCEL &dcel)
{
    for (int i = 0, numFaces = dcel.faces.size(); i < numFaces; i++) {
        triangulateMonotoneFace(dcel, i);
    }
}