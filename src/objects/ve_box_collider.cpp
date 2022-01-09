//
// Created by boris on 11/24/2021.
//

#include "ve_box_collider.h"

using namespace VE;

BoxCollider::BoxCollider(float width, float height, float depth, float mass, const Vector &localPosition)
        : Collider(ColliderType::box, mass),
          localVertices_(computeVertices(width, height, depth, localPosition)),
          localFaceNormals_(computeFaceNormals(localVertices_, indices_)),
          globalVertices_(localVertices_),
          globalFaceNormals_(localFaceNormals_),
          width_(width),
          height_(height),
          depth_(depth) {}

std::array<Vector, 8> BoxCollider::computeVertices(float width, float height, float depth, const Vector &localPosition) {
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float halfDepth = depth / 2;

    return {Vector(-halfWidth, -halfDepth, -halfHeight) + localPosition,
            Vector(halfWidth, -halfDepth, -halfHeight) + localPosition,
            Vector(halfWidth, halfDepth, -halfHeight) + localPosition,
            Vector(-halfWidth, halfDepth, -halfHeight) + localPosition,
            Vector(-halfWidth, -halfDepth, halfHeight) + localPosition,
            Vector(halfWidth, -halfDepth, halfHeight) + localPosition,
            Vector(halfWidth, halfDepth, halfHeight) + localPosition,
            Vector(-halfWidth, halfDepth, halfHeight) + localPosition};
}

std::array<Vector, 6> BoxCollider::computeFaceNormals(const std::array<Vector, 8> &vertices, const std::vector<unsigned int> &indices) {
    std::array<Vector, 6> faceNormals;
    for (unsigned int faceNumber = 0; faceNumber < 6; faceNumber++) {
        Vector AB = vertices[indices[4 * faceNumber + 1]] - vertices[indices[4 * faceNumber]];
        Vector BC = vertices[indices[4 * faceNumber + 2]] - vertices[indices[4 * faceNumber + 1]];

        faceNormals[faceNumber] = (AB * BC).normolize();
    }
    return faceNormals;
}

Matrix33 BoxCollider::getInertia() const {
    float tmp = mass_ / 12.0f;
    float sqrtWidth = width_ * width_;
    float sqrtHeight = height_ * height_;
    float sqrtDepth = depth_ * depth_;

    return Matrix33(
            tmp * (sqrtHeight + sqrtDepth), 0, 0,
            0, tmp * (sqrtWidth + sqrtDepth), 0,
            0, 0, tmp * (sqrtWidth + sqrtHeight)
    );
}

Vector BoxCollider::getCenterOfMass() const {
    return std::accumulate(localVertices_.begin(), localVertices_.end(), Vector()) / 8.0f;
}

Vector BoxCollider::farthestVertexInDirection(const Vector &direction) const {
    float l = std::numeric_limits<float>::lowest();

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
    return ColliderFace({globalVertices_[indices_[faceNumber * 4 + 0]],
                         globalVertices_[indices_[faceNumber * 4 + 1]],
                         globalVertices_[indices_[faceNumber * 4 + 2]],
                         globalVertices_[indices_[faceNumber * 4 + 3]]},
                        globalFaceNormals_[faceNumber]);
}

const void *BoxCollider::verticesGLFormatData() const {
    return reinterpret_cast<const void *>(globalVertices_.data());
}

const void *BoxCollider::indicesGLFormatData(unsigned int offset) const {
    return reinterpret_cast<const void *>(indices_.data() + offset);
}

unsigned int BoxCollider::indecesSize() const {
    return indices_.size();
}







