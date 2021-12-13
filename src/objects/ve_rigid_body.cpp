//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.h"

using namespace VE;

RigidBody::RigidBody() {
    angularVelocity_ = Vector(1, 0, 0) * M_PI_4;
}

RigidBody::~RigidBody() {

}

void RigidBody::addCollider(const VE::ColliderPtr &constShapePtr) {
    colliders_.emplace_back(constShapePtr);
    computeMass();

    Transform t;
    t.position = centerOfMass_ * -1;
    for (const auto &collider: colliders_) {
        collider->setLocalTransform(t);
    }
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

void RigidBody::computeMass() {
    float mass_ = 0;
    Matrix33 inertia;
    centerOfMass_ = Vector();
    for (const auto &collider: colliders_) {
        if (collider->mass() == 0) {
            invMass_ = 0;
            invInertia_ = Matrix33();
            centerOfMass_ = Vector();
            for (const auto &c: colliders_) {
                centerOfMass_ += c->getCenterOfMass();
            }
            centerOfMass_ = centerOfMass_ / static_cast<float>(colliders_.size());
            return;
        }
        mass_ += collider->mass();
        centerOfMass_ += collider->getCenterOfMass();
        inertia += collider->inertia();
    }
    centerOfMass_ = centerOfMass_ / static_cast<float>(colliders_.size());
    invInertia_ = inertia.getInverse();
    invMass_ = static_cast<float>(colliders_.size()) / mass_;
}

void RigidBody::update(float dt) {
    transform_.rotation += angularVelocity_ * dt;
}
