//
// Created by boris on 11/24/2021.
//

#include "ve_box_collider.h"

using namespace VE;

BoxCollider::BoxCollider(Transform localTransform) : BoxCollider(1.0f, 1.0f, 1.0f, localTransform) {

}

BoxCollider::BoxCollider(float width, float height, float depth, Transform localTransform) : Collider(
        ColliderType::box) {

    localVertices_ = {Vector(0, 0, 0),
                 Vector(width, 0, 0),
                 Vector(width, depth, 0),
                 Vector(0, depth, 0),
                 Vector(0, 0, height),
                 Vector(width, 0, height),
                 Vector(width, depth, height),
                 Vector(0, depth, height)};

    for (auto &vertex: localVertices_) {
        vertex = localTransform.apply(vertex);
    }
    globalVertices_ = localVertices_;

    computeCenterOfMass();
    setGlvertices();
}

Vector BoxCollider::farthestVertexInDirection(const Vector &direction) const {
    float l = -INFINITY;

    Vector lVertex;
    for (const VE::Vector &vertex: globalVertices_) {
        float l_tmp = vertex.dot(direction);
        if (l_tmp > l) {
            l = l_tmp;
            lVertex = vertex;
        }
    }
    return lVertex;
}

void BoxCollider::computeCenterOfMass() {
    Vector vertexSum;
    for (auto &vertex: localVertices_) {
        vertexSum += vertex;
    }
    localCenterOfMass_ = vertexSum / static_cast<float>(localVertices_.size());
}


void BoxCollider::setTransform(const Transform &transform) {
    for (size_t i = 0; i < localVertices_.size(); i++) {
        globalVertices_[i] = transform.apply(localVertices_[i]);
    }
    Collider::setTransform(transform);
}

void BoxCollider::setGlvertices() {
    glvertices_ = localVertices_;

    glindices_ = {
            3, 2, 1, 0,
            4, 5, 6, 7,
            0, 1, 5, 4,
            2, 3, 7, 6,
            3, 0, 4, 7,
            1, 2, 6, 5
    };
}





