//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.h"

using namespace VE;


RigidBodyPtr RigidBody::create(std::vector<ColliderPtr> &&collidersPtr) {
    return std::make_shared<VE::RigidBody>(std::forward<std::vector<ColliderPtr>>(collidersPtr));
}

RigidBody::RigidBody(std::vector<ColliderPtr> &&collidersPtr) : colliders_(collidersPtr) {
    computeMass();
    auto f = [&centerOfMass = centerOfMass_](ColliderPtr &c) {
        c->setLocalPosition(centerOfMass * -1);
    };
    std::for_each(colliders_.begin(), colliders_.end(), f);
    centerOfMass_.setZero();
}

RigidBody::~RigidBody() {
}

void RigidBody::computeMass() {
    float mass = 0.0f;
    Matrix33 inertia;
    centerOfMass_.setZero();

    bool infinityMass = false;
    for (const auto &collider: colliders_) {
        centerOfMass_ += collider->getCenterOfMass() * collider->mass();
        mass += collider->mass();
        inertia += collider->getInertia();

        if (collider->mass() == 0) {
            infinityMass = true;
        }
    }



    if (infinityMass) {
        invInertia_.setZero();
        invMass_ = 0.0f;
        centerOfMass_ /= static_cast<float>(colliders_.size());
    } else {
        invMass_ = 1 / mass;
        centerOfMass_ *= invMass_;
        for (const auto &collider: colliders_) {
            Vector r = centerOfMass_ - collider->getCenterOfMass();
            float sqrtX = r.x() * r.x();
            float sqrtY = r.y() * r.y();
            float sqrtZ = r.z() * r.z();

            inertia += Matrix33((sqrtY + sqrtZ) * collider->mass(), 0, 0,
                                0, (sqrtX + sqrtZ) * collider->mass(), 0,
                                0, 0, (sqrtX + sqrtY) * collider->mass());
        }
        invInertia_ = inertia.getInverse();
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
    centerOfMass_.drawPoint(12);
    if ((linearVelocity_ + pseudoLinearVelocity_).sqrtAbs() < (sleepEpsilont_ * sleepEpsilont_)) {
        linearVelocity_.setZero();
        pseudoLinearVelocity_.setZero();
    }

    if ((angularVelocity_ + pseudoAngularVelocity_).sqrtAbs() < (sleepEpsilont_ * sleepEpsilont_)) {
        angularVelocity_.setZero();
        pseudoAngularVelocity_.setZero();
    }

    transform_.position += (linearVelocity_ + pseudoLinearVelocity_) * dt;
    transform_.rotation += Quaternion((angularVelocity_ + pseudoAngularVelocity_) * 0.5f * dt) * transform_.rotation;
    transform_.rotation = transform_.rotation.normalization();

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
