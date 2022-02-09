//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_collider.h"

using namespace VE;

SphereCollider::SphereCollider(float radius, float mass, const Vector3 &localPosition) :
        Collider(ColliderType::sphere, mass),
        radius_(radius),
        localCenter_(localPosition),
        globalCenter_(localCenter_),
        renderVertices_(getGetRenderVertices(radius_, localCenter_)),
        renderIndices_(getGetRenderIndices()),
        renderNormals_(getRenderNormals(renderVertices_, renderIndices_)){}

ColliderPtr SphereCollider::create(float radius, float mass, const Vector3 &localPosition) {
    return std::make_shared<SphereCollider>(radius, mass, localPosition);
}

void SphereCollider::setLocalPosition(const Vector3 &localPosition) {
    localCenter_ += localPosition;
    globalCenter_ = localCenter_;
    renderVertices_ = getGetRenderVertices(radius_, localCenter_);
}

Matrix3 SphereCollider::getInertia() const {
    float tmp = 2.0f / 5.0f * mass_ * radius_ * radius_;
    return Matrix3(
            tmp, 0, 0,
            0, tmp, 0,
            0, 0, tmp
    );
}

Vector3 SphereCollider::farthestVertexInDirection(const Vector3 &direction) const {
    return direction.getNormalized() * radius_ + globalCenter_;
}

void SphereCollider::setTransform(const Transform &transform) {
    globalCenter_ = transform.applyToPoint(localCenter_);
}

Vector3 SphereCollider::getCenterOfMass() const {
    return globalCenter_;
}

float SphereCollider::getRadius() const {
    return radius_;
}

const std::vector<Vector3> &SphereCollider::vertices() const {
    return renderVertices_;
}

const std::vector<unsigned int> &SphereCollider::indices() const {
    return renderIndices_;
}

const std::vector<Vector3> &SphereCollider::normals() const {
    return renderNormals_;
}

std::vector<Vector3> SphereCollider::getGetRenderVertices(float radius, const Vector3 &position) {
    std::vector<Vector3> vertices;

    float dT = M_PI / nT;
    float dF = M_PI * 2 / nF;
    for (int i = 1; i < nT; i++) {
        for (int j = 0; j < nF; j++) {
            vertices.emplace_back(radius * sinf(dT * i) * cosf(dF * j) + position.x,
                                  radius * sinf(dT * i) * sinf(dF * j) + position.y,
                                  radius * cosf(dT * i) + position.z);

        }
    }

    return vertices;
}

std::vector<unsigned int> SphereCollider::getGetRenderIndices() {
    std::vector<unsigned int> indices;

    for (int i = 1; i < nT - 1; i++) {
        for (int j = 0; j < nF - 1; j++) {
            indices.push_back((i - 1) * nF + j); //0
            indices.push_back((i - 1) * nF + j + 1); //1
            indices.push_back(i * nF + j + 1); //2

            indices.push_back(i * nF + j + 1); //2
            indices.push_back(i * nF + j); //3
            indices.push_back((i - 1) * nF + j); //0
        }
        indices.push_back((i - 1) * nF + nF - 1); //0
        indices.push_back((i - 1) * nF + 0); //1
        indices.push_back(i * nF + 0); //2

        indices.push_back(i * nF + 0); //2
        indices.push_back(i * nF + nF - 1); //3
        indices.push_back((i - 1) * nF + nF - 1); //0
    }

    return indices;
}
