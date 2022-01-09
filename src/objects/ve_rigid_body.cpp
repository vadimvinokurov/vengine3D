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
    computeMass();

//    Transform t;
//    t.position = centerOfMass_ * -1;
//    for (const auto &collider: colliders_) {
//        collider->setLocalTransform(t);
//    }
//    centerOfMass_.setZero();
}

void RigidBody::computeMass() {
    float mass = 0.0f;
    Matrix33 inertia;
    centerOfMass_.setZero();

    bool infinityMass = false;
    for (const auto &collider: colliders_) {
        if (collider->mass() == 0) {
            infinityMass = true;
        }
        mass += collider->mass();
        centerOfMass_ += collider->getCenterOfMass();
        inertia += collider->getInertia();
    }
    centerOfMass_ = centerOfMass_ / static_cast<float>(colliders_.size());

    if (infinityMass) {
        invInertia_.setZero();
        invMass_ = 0.0f;
    } else {
        invInertia_ = inertia.getInverse();
        invMass_ = 1 / mass;
    }
}

const VE::Transform &RigidBody::transform() const {
    return transform_;
}

size_t RigidBody::collidersSize() const {
    return colliders_.size();
}

const Collider &RigidBody::collider(size_t n) const {
    return *colliders_[n];
}

void RigidBody::setTransform(const Transform &transform) {
    transform_ = transform;
    for (auto &collider: colliders_) {
        collider->setTransform(transform_);
    }
}

void RigidBody::updateVelocity(float dt) {
    angularVelocity_ += invInertia_ * torque_ * dt;
    linearVelocity_ += force_ * invMass_ * dt + gravity_ * dt;

    force_.setZero();
    torque_.setZero();
}

void RigidBody::updateTransform(float dt) {
    if ((linearVelocity_ + pseudoLinearVelocity_).sqrtAbs() < (sleepEpsilont_ * sleepEpsilont_)) {
        linearVelocity_.setZero();
        pseudoLinearVelocity_.setZero();
    }

    if ((angularVelocity_ + pseudoAngularVelocity_).sqrtAbs() < (sleepEpsilont_ * sleepEpsilont_)) {
        angularVelocity_.setZero();
        pseudoAngularVelocity_.setZero();
    }

    transform_.position += (linearVelocity_ + pseudoLinearVelocity_) * dt;
    transform_.rotation = transform_.rotation + Quaternion((angularVelocity_ + pseudoAngularVelocity_) * dt * 0.5f) * transform_.rotation;

    setTransform(transform_);

    linearVelocity_ *= damping_;
    angularVelocity_ *= damping_;
    pseudoLinearVelocity_.setZero();
    pseudoAngularVelocity_.setZero();
}

void RigidBody::addForce(const Vector &force) {
    force_ += force_;
}

void RigidBody::addForce(const Vector &force, const Vector &forcePoint) {
    force_ += force;
    torque_ += (forcePoint - centerOfMass()) * force;
}

void RigidBody::setLinearVelocity(const Vector &linearVelocity) {
    linearVelocity_ = linearVelocity;
}

void RigidBody::setAngularVelocity(const Vector &angularVelocity) {
    angularVelocity_ = angularVelocity;
}

const Vector &RigidBody::linearVelocity() const {
    return linearVelocity_;
}

const Vector &RigidBody::angularVelocity() const {
    return angularVelocity_;
}

void RigidBody::setPseudoLinearVelocity(const Vector &pseudoLinearVelocity) {
    pseudoLinearVelocity_ = pseudoLinearVelocity;
}

void RigidBody::setPseudoAngularVelocity(const Vector &pseudoAngularVelocity) {
    pseudoAngularVelocity_ = pseudoAngularVelocity;
}

const Vector &RigidBody::pseudoLinearVelocity() const {
    return pseudoLinearVelocity_;
}

const Vector &RigidBody::pseudoAngularVelocity() const {
    return pseudoAngularVelocity_;
}

void RigidBody::setGravity(const Vector &gravity) {
    gravity_ = gravity;
}

float RigidBody::invMass() const {
    return invMass_;
}

float RigidBody::restitution() const {
    return restitution_;
}

const Vector &RigidBody::centerOfMass() const {
    return transform_.position;
}

const Matrix33 &RigidBody::invInertia() const {
    return invInertia_;
}

Vector RigidBody::globalToLocalPoint(const Vector &globalPoint) {
    return transform_.applyInverse(globalPoint);
}

Vector RigidBody::localToGlobalPoint(const Vector &localPoint) {
    return transform_.apply(localPoint);
}

float RigidBody::friction() const {
    return friction_;
}

void RigidBody::setRestitution(float restitution) {
    restitution_ = restitution;
}

void RigidBody::setPosition(const Vector &position) {
    transform_.position = position;
}
