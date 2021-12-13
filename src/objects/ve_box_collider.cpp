//
// Created by boris on 11/24/2021.
//

#include "ve_box_collider.h"

using namespace VE;

BoxCollider::BoxCollider(float width, float height, float depth, float mass) : Collider(ColliderType::box) {

    localVertices_ = {Vector(0, 0, 0),
                      Vector(width, 0, 0),
                      Vector(width, depth, 0),
                      Vector(0, depth, 0),
                      Vector(0, 0, height),
                      Vector(width, 0, height),
                      Vector(width, depth, height),
                      Vector(0, depth, height)};


    mass_ = mass;
    computeFaceNormals();
    computeCenterOfMass();
    initGlobalBuffer();
    computeBoxInertia(width, height, depth);
}

void BoxCollider::setLocalTransform(const Transform &localTransform) {
    for (auto &vertex: localVertices_) {
        vertex = localTransform.apply(vertex);
    }
    computeFaceNormals();
    computeCenterOfMass();
    initGlobalBuffer();
}

void BoxCollider::computeBoxInertia(float width, float height, float depth) {
    float tmp = mass_ / 12.0f;
    float dWidth = width * width;
    float dHeight = height * height;
    float dDepth = depth * depth;

    inertia_ = Matrix33(
            tmp * (dHeight + dDepth), 0, 0,
            0, tmp * (dWidth + dDepth), 0,
            0, 0, tmp * (dWidth + dHeight)
    );
}

void BoxCollider::computeFaceNormals() {
    localFaceNormals_.clear();
    for (unsigned int faceNumber = 0; faceNumber < 6; faceNumber++) {
        Vector AB = localVertices_[indices_[4 * faceNumber + 1]] - localVertices_[indices_[4 * faceNumber]];
        Vector BC = localVertices_[indices_[4 * faceNumber + 2]] - localVertices_[indices_[4 * faceNumber + 1]];

        localFaceNormals_.emplace_back((AB * BC).normolize());
    }
}

void BoxCollider::computeCenterOfMass() {
    Vector vertexSum;
    for (auto &vertex: localVertices_) {
        vertexSum += vertex;
    }
    centerOfMass_ = vertexSum / static_cast<float>(localVertices_.size());
}

void BoxCollider::initGlobalBuffer() {
    globalVertices_ = localVertices_;
    globalFaceNormals_ = localFaceNormals_;
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


void BoxCollider::setTransform(const Transform &transform) {
    for (size_t i = 0; i < localVertices_.size(); i++) {
        globalVertices_[i] = transform.apply(localVertices_[i]);
    }
    for (size_t i = 0; i < localFaceNormals_.size(); i++) {
        globalFaceNormals_[i] = transform.applyForNormal(localFaceNormals_[i]);
    }
    Collider::setTransform(transform);
}

ColliderFace BoxCollider::getFaceInDirection(const Vector &direction) const {
    float projectionMax = std::numeric_limits<float>::lowest();
    unsigned int selectedFaceNumber = std::numeric_limits<unsigned int>::max();

    for (unsigned int faceNumber = 0; faceNumber < 6; faceNumber++) {
        float projection = direction.dot(globalFaceNormals_[faceNumber]);
        if (projection > projectionMax) {
            projectionMax = projection;
            selectedFaceNumber = faceNumber;
        }
    }
    assert(selectedFaceNumber != std::numeric_limits<unsigned int>::max());

    return getFace(selectedFaceNumber);
}

ColliderFace BoxCollider::getFace(unsigned int faceNumber) const {
    assert((faceNumber * 4 + 3) < indices_.size());
    return ColliderFace({globalVertices_[indices_[faceNumber * 4 + 0]], globalVertices_[indices_[faceNumber * 4 + 1]],
                         globalVertices_[indices_[faceNumber * 4 + 2]], globalVertices_[indices_[faceNumber * 4 + 3]]},
                        globalFaceNormals_[faceNumber]);
}

const void *BoxCollider::verticesGLFormatData() const {
    return reinterpret_cast<const void *>(localVertices_.data());
}

const void *BoxCollider::indicesGLFormatData(unsigned int offset) const {
    return reinterpret_cast<const void *>(indices_.data() + offset);
}

unsigned int BoxCollider::indecesSize() const {
    return indices_.size();
}





