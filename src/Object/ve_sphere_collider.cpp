//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_collider.h"

using namespace VE;

SphereCollider::SphereCollider(Vector center) : SphereCollider(1.0f, center) {

}

SphereCollider::SphereCollider(float radius, Vector center) : Collider(ColliderType::sphere),
                                                              radius_(radius) {
    localCenterOfMass_ = center;
    setGlvertices();
}

Vector SphereCollider::farthestVertexInDirection(const Vector &direction) const {
    return direction * radius_ + globalCenterOfMass_;
}

void SphereCollider::setGlvertices() {
    int nT = 10;
    int nF = 10;

    float dT = M_PI / nT;
    float dF = M_PI * 2 / nF;
    for (int i = 1; i < nT; i++) {
        for (int j = 0; j < nF; j++) {
            glvertices_.emplace_back(radius_ * sinf(dT * i) * cosf(dF * j),
                                     radius_ * sinf(dT * i) * sinf(dF * j),
                                     radius_ * cosf(dT * i));

        }
    }

    for (int i = 1; i < nT - 1; i++) {
        for (int j = 0; j < nF - 1; j++) {
            glindices_.push_back((i - 1) * nF + j);
            glindices_.push_back((i - 1) * nF + j + 1);
            glindices_.push_back(i * nF + j + 1);
            glindices_.push_back(i * nF + j);
        }
        glindices_.push_back((i - 1) * nF + nF - 1);
        glindices_.push_back((i - 1) * nF + 0);
        glindices_.push_back(i * nF + 0);
        glindices_.push_back(i * nF + nF - 1);
    }
}

