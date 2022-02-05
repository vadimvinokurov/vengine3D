//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_collider.h"

using namespace VE;

SphereCollider::SphereCollider(float radius, float mass, const Vector &localPosition) :
        Collider(ColliderType::sphere, mass),
        radius_(radius),
        localCenter_(localPosition),
        globalCenter_(localCenter_) {
    setGlvertices();
}

ColliderPtr SphereCollider::create(float radius, float mass, const Vector &localPosition) {
    return std::make_shared<SphereCollider>(radius, mass, localPosition);
}

void SphereCollider::setLocalPosition(const Vector &localPosition) {
    localCenter_ += localPosition;
    globalCenter_ = localCenter_;
    setGlvertices();
}

Matrix33 SphereCollider::getInertia() const {
    float tmp = 2.0f / 5.0f * mass_ * radius_ * radius_;
    return Matrix33(
            tmp, 0, 0,
            0, tmp, 0,
            0, 0, tmp
    );
}

Vector SphereCollider::farthestVertexInDirection(const Vector &direction) const {
    return direction.normalized() * radius_ + globalCenter_;
}

void SphereCollider::setTransform(const Transform &transform) {
    globalCenter_ = transform.apply(localCenter_);
}

Vector SphereCollider::getCenterOfMass() const {
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
            glIndicesBuffer_.push_back((i - 1) * nF + j);
            glIndicesBuffer_.push_back((i - 1) * nF + j + 1);
            glIndicesBuffer_.push_back(i * nF + j + 1);
            glIndicesBuffer_.push_back(i * nF + j);
        }
        glIndicesBuffer_.push_back((i - 1) * nF + nF - 1);
        glIndicesBuffer_.push_back((i - 1) * nF + 0);
        glIndicesBuffer_.push_back(i * nF + 0);
        glIndicesBuffer_.push_back(i * nF + nF - 1);
    }
}

const void *SphereCollider::verticesGLFormatData() const {
    return reinterpret_cast<const void *>(glVerticesBuffer_.data());
}

const void *SphereCollider::indicesGLFormatData(unsigned int offset) const {
    return reinterpret_cast<const void *>(glIndicesBuffer_.data() + offset);
}

unsigned int SphereCollider::indecesSize() const {
    return glIndicesBuffer_.size();
}

float SphereCollider::getRadius() const {
    return radius_;
}
