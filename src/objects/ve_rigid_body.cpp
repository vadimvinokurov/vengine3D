//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.h"

using namespace VE;


RigidBodyPtr RigidBody::create(std::vector<ColliderPtr> &&collidersPtr) {
    return std::make_shared<VE::RigidBody>(std::move(collidersPtr));
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
    Matrix3 inertia;
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
            Vector3 r = centerOfMass_ - collider->getCenterOfMass();
            float sqrtX = r.x * r.x;
            float sqrtY = r.y * r.y;
            float sqrtZ = r.z * r.z;

            inertia += Matrix3((sqrtY + sqrtZ) * collider->mass(), 0, 0,
                                0, (sqrtX + sqrtZ) * collider->mass(), 0,
                                0, 0, (sqrtX + sqrtY) * collider->mass());
        }
        invInertia_ = inertia.getInversed();
    }
}

const VE::Transform &RigidBody::transform() const {
    return transform_;
}

std::vector<ColliderPtr> &RigidBody::colliders() {
    return colliders_;
}

const std::vector<ColliderPtr> &RigidBody::colliders() const {
    return colliders_;
}

void RigidBody::setTransform(const Transform &transform) {
    Transform t = transform * invtransform_;
    for (auto &collider: colliders_) {
        collider->setTransform(t);
    }
    transform_ = transform;
    invtransform_ = transform.getInversed();
}

void RigidBody::updateVelocity(float dt) {
    angularVelocity_ += invInertia_ * torque_ * dt;
    linearVelocity_ += force_ * invMass_ * dt + gravity_ * dt;

    force_.setZero();
    torque_.setZero();
}

void RigidBody::updateTransform(float dt) {
    if ((linearVelocity_ + pseudoLinearVelocity_).lenSqrt() < (sleepEpsilont_ * sleepEpsilont_)) {
        linearVelocity_.setZero();
        pseudoLinearVelocity_.setZero();
    }

    if ((angularVelocity_ + pseudoAngularVelocity_).lenSqrt() < (sleepEpsilont_ * sleepEpsilont_)) {
        angularVelocity_.setZero();
        pseudoAngularVelocity_.setZero();
    }

    transform_.position += (linearVelocity_ + pseudoLinearVelocity_) * dt;
    transform_.rotation += Quaternion((angularVelocity_ + pseudoAngularVelocity_) * 0.5f * dt) * transform_.rotation;
    transform_.rotation = transform_.rotation.getNormalized();

    setTransform(transform_);

    linearVelocity_ *= damping_;
    angularVelocity_ *= damping_;
    pseudoLinearVelocity_.setZero();
    pseudoAngularVelocity_.setZero();
}

void RigidBody::addForce(const Vector3 &force) {
    force_ += force_;
}

void RigidBody::addForce(const Vector3 &force, const Vector3 &forcePoint) {
    force_ += force;
    torque_ += (forcePoint - centerOfMass()) * force;
}

void RigidBody::setLinearVelocity(const Vector3 &linearVelocity) {
    linearVelocity_ = linearVelocity;
}

void RigidBody::setAngularVelocity(const Vector3 &angularVelocity) {
    angularVelocity_ = angularVelocity;
}

const Vector3 &RigidBody::linearVelocity() const {
    return linearVelocity_;
}

const Vector3 &RigidBody::angularVelocity() const {
    return angularVelocity_;
}

void RigidBody::setPseudoLinearVelocity(const Vector3 &pseudoLinearVelocity) {
    pseudoLinearVelocity_ = pseudoLinearVelocity;
}

void RigidBody::setPseudoAngularVelocity(const Vector3 &pseudoAngularVelocity) {
    pseudoAngularVelocity_ = pseudoAngularVelocity;
}

const Vector3 &RigidBody::pseudoLinearVelocity() const {
    return pseudoLinearVelocity_;
}

const Vector3 &RigidBody::pseudoAngularVelocity() const {
    return pseudoAngularVelocity_;
}

void RigidBody::setGravity(const Vector3 &gravity) {
    gravity_ = gravity;
}

float RigidBody::invMass() const {
    return invMass_;
}

float RigidBody::restitution() const {
    return restitution_;
}

const Vector3 &RigidBody::centerOfMass() const {
    return transform_.position;
}

const Matrix3 &RigidBody::invInertia() const {
    return invInertia_;
}

Vector3 RigidBody::globalToLocalPoint(const Vector3 &globalPoint) {
    return transform_.getInversed().applyToPoint(globalPoint);
}

Vector3 RigidBody::localToGlobalPoint(const Vector3 &localPoint) {
    return transform_.applyToPoint(localPoint);
}

float RigidBody::friction() const {
    return friction_;
}

void RigidBody::setRestitution(float restitution) {
    restitution_ = restitution;
}

void RigidBody::setPosition(const Vector3 &position) {
    transform_.position = position;
}
