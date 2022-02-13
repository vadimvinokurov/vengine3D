//
// Created by boris on 11/29/2021.
//

#include "ve_epa.h"
#include "collision/ve_support_point.h"
#include "ve_engine_settings.h"

using namespace VE;

EPA::PenetrationDepth::PenetrationDepth(const Collider &collider1, const Collider &collider2, const std::vector<Vector3> &simplex): collider1_(collider1),
                                                                                                                                    collider2_(collider2),
                                                                                                                                    polytope_(simplex) {

}

Vector3 EPA::PenetrationDepth::getVector() {
	int interuptCount = 0;
    while(true){
        const Face &closestFaceToOrigin = polytope_.getClosestFaceToOrigin();
        Vector3 extendVertex = getSupportPoint(collider1_, collider2_, closestFaceToOrigin.normal).point;
        float extendVertexDistance = extendVertex.dot(closestFaceToOrigin.normal);
        if (abs(extendVertexDistance - closestFaceToOrigin.distance) > Collision::tolerance) {
            computeNewFaces(extendVertex);
        } else {
            return closestFaceToOrigin.normal * (closestFaceToOrigin.distance + Collision::tolerance);
        }
		
		if(++interuptCount > VEngineSettings::INFINITE_LOOP_INTERUPT){
            std::cerr << "Vector3 EPA::PenetrationDepth::getVector() infinity loop" << std::endl;
			return Vector3(0,0,1);
		}
    }
}

void EPA::PenetrationDepth::computeNewFaces(const Vector3 &support) {
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
