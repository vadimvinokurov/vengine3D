//
// Created by boris on 11/18/2021.
//

#include "ve_camera.h"

using namespace VE;

Camera::Camera() {
    position_ = Vector(0, -17.5, 0);
    cameraDirection_ = Vector(0, -1, 0);
    cameraUp_ = Vector(0, 0, 1);
    cameraRight_ = Vector(1, 0, 0);
}

void Camera::moveAlongDirection(float distance) {
    position_ += cameraDirection_ * distance * -1;
}

void Camera::moveAlongSide(float distance) {
    position_ += cameraRight_ * distance;
}

const Vector &Camera::position() const {
    return position_;
}

const Vector &Camera::direction() const {
    return cameraDirection_;
}

const std::array<float, 16> &Camera::getViewMatrix() const {
    const Vector &R = cameraRight_;
    const Vector &U = cameraUp_;
    const Vector &D = cameraDirection_;
    const Vector &P = position_;

    viewMatrix_ = {cameraRight_.x(), cameraUp_.x(), cameraDirection_.x(), 0,
                   cameraRight_.y(), cameraUp_.y(), cameraDirection_.y(), 0,
                   cameraRight_.z(), cameraUp_.z(), cameraDirection_.z(), 0,
                   -R.x() * P.x() - R.y() * P.y() - R.z() * P.z(),
                   -U.x() * P.x() - U.y() * P.y() - U.z() * P.z(),
                   -D.x() * P.x() - D.y() * P.y() - D.z() * P.z(),
                   1};

    return viewMatrix_;
}

Camera::~Camera() {

}

void Camera::setDirection(float pitch, float yam) {
    static float p = 0;
    static float y = 0;
    p += pitch * 10;
    y += yam * 10;
    std::cout << p << " " << y << std::endl;
    auto radians = [](float angle) { return angle / 180.0f; };
    cameraDirection_.setX(cosf(radians(p)) * cosf(radians(y)));
    cameraDirection_.setY(cosf(radians(p)) * sinf(radians(y)));
    cameraDirection_.setZ(sinf(radians(p)));
}
