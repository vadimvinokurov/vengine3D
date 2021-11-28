//
// Created by boris on 11/18/2021.
//

#include "ve_collider.h"

using namespace VE;

Collider::Collider(ColliderType shapeType) : shapeType_(shapeType),
                                             color_(0.5, 0.5, 0.5) {

}

const float *Collider::verticesGLFormatData() const {
    return reinterpret_cast<const float *>(glvertices_.data());
}

const void *Collider::indicesGLFormatData(unsigned int offset) const {
    return static_cast<const void *>(glindices_.data() + offset);
}

Collider::~Collider() {

}

ColliderType Collider::shapeType() const {
    return shapeType_;
}

unsigned int Collider::indecesSize() const {
    return glindices_.size();
}

void Collider::setTransform(const Transform &transform) {
    globalCenterOfMass_ = transform.apply(localCenterOfMass_);
}

void Collider::setColor(const Color &color) const {
    color_ = color;
}

Color Collider::color() const {
    return color_;
}

