//
// Created by boris on 11/29/2021.
//

#include "ve_epa.h"
#include "collision/ve_support_point.h"
#include "ve_stdlibs.h"
#include "ve_global_parameters.h"

using namespace VE;

EPA::PenetrationDepth::PenetrationDepth(const Collider &collider1, const Collider &collider2, const std::vector<Vector> &simplex): collider1_(collider1),
                                                                                                                                   collider2_(collider2),
                                                                                                                                   polytope_(simplex) {

}

Vector EPA::PenetrationDepth::getVector() {
    while(true){
        const Face &closestFaceToOrigin = polytope_.getClosestFaceToOrigin();
        Vector extendVertex = getSupportPoint(collider1_, collider2_, closestFaceToOrigin.normal).point;
        float extendVertexDistance = extendVertex.dot(closestFaceToOrigin.normal);
        if (abs(extendVertexDistance - closestFaceToOrigin.distance) > Collision::tolerance) {
            computeNewFaces(extendVertex);
        } else {
            return closestFaceToOrigin.normal * (closestFaceToOrigin.distance + Collision::tolerance);
        }
    }
}

void EPA::PenetrationDepth::computeNewFaces(const Vector &support) {
    UnigueEdge unigueEdge;
    for (size_t i = 0; i < polytope_.getFacesSize(); i++) {
        if (sameDirection(polytope_[i].normal, support - polytope_.getFaceVertex(i, 0))) {
            unigueEdge.add(polytope_[i].index[0], polytope_[i].index[1]);
            unigueEdge.add(polytope_[i].index[1], polytope_[i].index[2]);
            unigueEdge.add(polytope_[i].index[2], polytope_[i].index[0]);
            polytope_.deleteFace(i);
            i--;
        }
    }

    polytope_.addVertex(support);
    for (auto[i0, i1]: unigueEdge.edges()) {
        polytope_.addFace(Face(i0, i1, polytope_.getVerticesSize() - 1));
    }
}
