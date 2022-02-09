//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_collider.h"

using namespace VE;

SphereCollider::SphereCollider(float radius, float mass, const Vector3 &localPosition) :
        Collider(ColliderType::sphere, mass),
        radius_(radius),
        localCenter_(localPosition),
        globalCenter_(localCenter_) {
    setGlvertices();
}

ColliderPtr SphereCollider::create(float radius, float mass, const Vector3 &localPosition) {
    return std::make_shared<SphereCollider>(radius, mass, localPosition);
}

void SphereCollider::setLocalPosition(const Vector3 &localPosition) {
    localCenter_ += localPosition;
    globalCenter_ = localCenter_;
    setGlvertices();
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

void SphereCollider::setGlvertices() {
    glVerticesBuffer_.clear();
    glIndicesBuffer_.clear();

    int nT = 10;
    int nF = 10;
    float dT = M_PI / nT;
    float dF = M_PI * 2 / nF;
    for (int i = 1; i < nT; i++) {
        for (int j = 0; j < nF; j++) {
            glVerticesBuffer_.emplace_back(radius_ * sinf(dT * i) * cosf(dF * j) + localCenter_.x,
                                           radius_ * sinf(dT * i) * sinf(dF * j) + localCenter_.y,
                                           radius_ * cosf(dT * i) + localCenter_.z);

        }
    }

    for (int i = 1; i < nT - 1; i++) {
        for (int j = 0; j < nF - 1; j++) {
            glIndicesBuffer_.push_back((i - 1) * nF + j); //0
            glIndicesBuffer_.push_back((i - 1) * nF + j + 1); //1
            glIndicesBuffer_.push_back(i * nF + j + 1); //2

            glIndicesBuffer_.push_back(i * nF + j + 1); //2
            glIndicesBuffer_.push_back(i * nF + j); //3
            glIndicesBuffer_.push_back((i - 1) * nF + j); //0
        }
        glIndicesBuffer_.push_back((i - 1) * nF + nF - 1); //0
        glIndicesBuffer_.push_back((i - 1) * nF + 0); //1
        glIndicesBuffer_.push_back(i * nF + 0); //2

        glIndicesBuffer_.push_back(i * nF + 0); //2
        glIndicesBuffer_.push_back(i * nF + nF - 1); //3
        glIndicesBuffer_.push_back((i - 1) * nF + nF - 1); //0
    }
}

const std::vector<Vector3> &SphereCollider::vertices() const {
    return glVerticesBuffer_;
}

const std::vector<unsigned int> &SphereCollider::indices() const {
    return glIndicesBuffer_;
}

float SphereCollider::getRadius() const {
    return radius_;
}

