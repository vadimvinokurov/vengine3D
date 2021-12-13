//
// Created by boris on 11/18/2021.
//

#include "ve_collider.h"

using namespace VE;

Collider::Collider(ColliderType shapeType) : shapeType_(shapeType),
                                             color_(0.5, 0.5, 0.5) {

}

Collider::~Collider() {

}

ColliderType Collider::shapeType() const {
    return shapeType_;
}

void Collider::setTransform(const Transform &transform) {
    transform_ = transform;
}

void Collider::setColor(const Color &color) const {
    color_ = color;
}

Color Collider::color() const {
    return color_;
}

const Transform &Collider::transform() const {
    return transform_;
}

float Collider::mass() const {
    return mass_;
}

const Matrix33 &Collider::inertia() const {
    return inertia_;
}

const Vector &Collider::centerOfMass() const {
    return centerOfMass_;
}

void Collider::setLocalTransform(const Transform &localTransform) {
    centerOfMass_ = localTransform.apply(centerOfMass_);
}

