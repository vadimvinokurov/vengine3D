//
// Created by boris on 11/18/2021.
//

#include "ve_camera.h"

VE::Camera::Camera() {
    position_ = Vector(6.5, -7.5, 4);
    rotation_ = Vector(70, 0, 40);
}


VE::Camera::~Camera() {

}

void VE::Camera::setPosition(const VE::Vector &position) {
    position_ = position;
}

void VE::Camera::setRotation(const VE::Vector &rotation) {
    rotation_ = rotation;
}

const VE::Vector &VE::Camera::position() const {
    return position_;
}

const VE::Vector &VE::Camera::rotation() const {
    return rotation_;
}

void VE::Camera::setDeltaPosition(const VE::Vector &dPosition) {
    position_ += dPosition;
}

void VE::Camera::setDeltaRotation(const VE::Vector &dRotation) {
    rotation_ += dRotation;
}
