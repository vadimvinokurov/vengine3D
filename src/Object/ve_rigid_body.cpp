//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.hpp"

using namespace VE;

RigidBody::RigidBody() {

}

RigidBody::~RigidBody() {

}

void RigidBody::addCollider(const VE::ConstShapePtr &constShapePtr) {

    colliders_.emplace_back(constShapePtr);
}

void RigidBody::moveTo(VE::Vector dp) {
    transform_.position += dp;
}

const VE::Transform &RigidBody::transform() const {
    return transform_;
}

size_t RigidBody::collidersSize() {
    return colliders_.size();
}

const Shape &RigidBody::collider(size_t n) {
    return *colliders_[n];
}

void RigidBody::setTransform(const Transform &transform) {
    transform_ = transform;
}
