//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.h"

using namespace VE;

RigidBody::RigidBody() {

}

RigidBody::~RigidBody() {

}

void RigidBody::addCollider(const VE::ColliderPtr &constShapePtr) {

    colliders_.emplace_back(constShapePtr);
}

void RigidBody::moveTo(VE::Vector dp) {
    transform_.position += dp;
    setTransform(transform_);
}

const VE::Transform &RigidBody::transform() const {
    return transform_;
}

size_t RigidBody::collidersSize() {
    return colliders_.size();
}

const Collider &RigidBody::collider(size_t n) {
    return *colliders_[n];
}

void RigidBody::setTransform(const Transform &transform) {
    transform_ = transform;
    for (auto &collider: colliders_) {
        collider->setTransform(transform_);
    }
}
