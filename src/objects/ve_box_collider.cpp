//
// Created by boris on 11/24/2021.
//

#include "ve_box_collider.h"

using namespace VE;

ColliderPtr BoxCollider::create(float width, float height, float depth, float mass, const Vector3 &localPosition) {
    return std::make_shared<VE::BoxCollider>(width, height, depth, mass, localPosition);
}

BoxCollider::BoxCollider(float width, float height, float depth, float mass, const Vector3 &localPosition)
        : Collider(ColliderType::box, mass),
          localVertices_(computeVertices(width, height, depth, localPosition)),
          localFaceNormals_(computeFaceNormals(localVertices_, indices_)),
          globalVertices_(localVertices_),
          globalFaceNormals_(localFaceNormals_),
          localCenterOfMass_(computeCenterOfMass(localVertices_)),
          globalCenterOfMass_(localCenterOfMass_),
          width_(width),
          height_(height),
          depth_(depth),
          renderNormals_(getRenderNormals(localVertices_, renderIndices_)){}

void BoxCollider::setLocalPosition(const Vector3 &localPosition) {
    std::for_each(localVertices_.begin(), localVertices_.end(), [&localPosition](Vector3 &v) { v += localPosition; });
    globalVertices_ = localVertices_;
    localCenterOfMass_ = computeCenterOfMass(localVertices_);
    globalCenterOfMass_ = localCenterOfMass_;
}

std::vector<Vector3> BoxCollider::computeVertices(float width, float height, float depth, const Vector3 &localPosition) {
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float halfDepth = depth / 2;

    return {Vector3(-halfWidth, -halfDepth, -halfHeight) + localPosition,
            Vector3(halfWidth, -halfDepth, -halfHeight) + localPosition,
            Vector3(halfWidth, halfDepth, -halfHeight) + localPosition,
            Vector3(-halfWidth, halfDepth, -halfHeight) + localPosition,
            Vector3(-halfWidth, -halfDepth, halfHeight) + localPosition,
            Vector3(halfWidth, -halfDepth, halfHeight) + localPosition,
            Vector3(halfWidth, halfDepth, halfHeight) + localPosition,
            Vector3(-halfWidth, halfDepth, halfHeight) + localPosition};
}

std::vector<Vector3> BoxCollider::computeFaceNormals(const std::vector<Vector3> &vertices, const std::vector<unsigned int> &indices) {
    std::vector<Vector3> faceNormals(6);
    for (unsigned int faceNumber = 0; faceNumber < 6; faceNumber++) {
        Vector3 AB = vertices[indices[4 * faceNumber + 1]] - vertices[indices[4 * faceNumber]];
        Vector3 BC = vertices[indices[4 * faceNumber + 2]] - vertices[indices[4 * faceNumber + 1]];

        faceNormals[faceNumber] = (AB * BC).getNormalized();
    }
    return faceNormals;
}

Vector3 BoxCollider::computeCenterOfMass(const std::vector<Vector3> &vertices) {
    return std::accumulate(vertices.begin(), vertices.end(), Vector3()) / vertices.size();
}


Matrix3 BoxCollider::getInertia() const {
    float tmp = mass_ / 12.0f;
    float sqrtWidth = width_ * width_;
    float sqrtHeight = height_ * height_;
    float sqrtDepth = depth_ * depth_;

    return Matrix3(
            tmp * (sqrtHeight + sqrtDepth), 0, 0,
            0, tmp * (sqrtWidth + sqrtDepth), 0,
            0, 0, tmp * (sqrtWidth + sqrtHeight)
    );
}

Vector3 BoxCollider::getCenterOfMass() const {
    return globalCenterOfMass_;
}

Vector3 BoxCollider::farthestVertexInDirection(const Vector3 &direction) const {
    float l = std::numeric_limits<float>::lowest();

    Vector3 lVertex;
    for (const VE::Vector3 &vertex: globalVertices_) {
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
        globalVertices_[i] = transform.applyToPoint(localVertices_[i]);
    }
    for (size_t i = 0; i < localFaceNormals_.size(); i++) {
        globalFaceNormals_[i] = transform.applyToVector(localFaceNormals_[i]);
    }
    globalCenterOfMass_ = transform.applyToPoint(localCenterOfMass_);
}

ColliderFace BoxCollider::getFaceInDirection(const Vector3 &direction) const {
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

const std::vector<Vector3> &BoxCollider::vertices() const {
    return localVertices_;
}

const std::vector<unsigned int> &BoxCollider::indices() const {
    return renderIndices_;
}

const std::vector<Vector3> &BoxCollider::normals() const {
    return renderNormals_;
}




