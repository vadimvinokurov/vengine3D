//
// Created by boris on 11/25/2021.
//

#include "ve_rigid_body.hpp"

VE::RigidBody::RigidBody() {

}

VE::RigidBody::~RigidBody() {

}

void VE::RigidBody::addCollider(const VE::ShapePtr &shapePtr) {
    colliders_.emplace_back(shapePtr);
}

void VE::RigidBody::moveTo(VE::Vector dp) {
    transform_.position += dp;
}
