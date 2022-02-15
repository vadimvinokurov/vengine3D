//
// Created by boris on 11/24/2021.
//

#include "ve_box_collider.h"

using namespace VE;

BoxCollider::BoxCollider(float width, float height, float depth, float mass, const Vector3 &localPosition)
        : Collider(ColliderType::box, mass),
          vertices_(computeVertices(width, height, depth, localPosition)),
          faceNormals_(computeFaceNormals(vertices_, indices_)),
          centerOfMass_(computeCenterOfMass(vertices_)),
          width_(width),
          height_(height),
          depth_(depth) {

    vertexPosition.set(vertices_);
    indexBuffer.set(renderIndices_);
    auto normals = getRenderNormals(vertices_, renderIndices_);
    vertexNormals.set(normals);

}

void BoxCollider::setLocalPosition(const Vector3 &localPosition) {
    std::for_each(vertices_.begin(), vertices_.end(), [&localPosition](Vector3 &v) { v += localPosition; });
    centerOfMass_ = computeCenterOfMass(vertices_);

    vertexPosition.set(vertices_);
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
    return centerOfMass_;
}

Vector3 BoxCollider::farthestVertexInDirection(const Vector3 &direction) const {
    float l = std::numeric_limits<float>::lowest();

    Vector3 lVertex;
    for (const VE::Vector3 &vertex: vertices_) {
        float l_tmp = vertex.dot(direction);
        if (l_tmp > l) {
            l = l_tmp;
            lVertex = vertex;
        }
    }
    return lVertex;
}

void BoxCollider::setTransform(const Transform &transform) {
    for (size_t i = 0; i < vertices_.size(); i++) {
        vertices_[i] = transform.applyToPoint(vertices_[i]);
    }
    for (size_t i = 0; i < faceNormals_.size(); i++) {
        faceNormals_[i] = transform.applyToVector(faceNormals_[i]);
    }
    centerOfMass_ = transform.applyToPoint(centerOfMass_);
}

ColliderFace BoxCollider::getFaceInDirection(const Vector3 &direction) const {
    float projectionMax = std::numeric_limits<float>::lowest();
    unsigned int selectedFaceNumber = std::numeric_limits<unsigned int>::max();

    for (unsigned int faceNumber = 0; faceNumber < 6; faceNumber++) {
        float projection = direction.dot(faceNormals_[faceNumber]);
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
    return ColliderFace({vertices_[indices_[faceNumber * 4 + 0]],
                         vertices_[indices_[faceNumber * 4 + 1]],
                         vertices_[indices_[faceNumber * 4 + 2]],
                         vertices_[indices_[faceNumber * 4 + 3]]},
                        faceNormals_[faceNumber]);
}



