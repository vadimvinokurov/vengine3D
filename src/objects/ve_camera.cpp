//
// Created by boris on 11/18/2021.
//

#include "ve_camera.h"
#include "math/ve_matrix33.h"

using namespace VE;

Camera::Camera() : position_(CameraParameters::defaultCameraPosition),
                   cameraDirection_(CameraParameters::defaultCameraDirection),
                   cameraUp_(CameraParameters::defaultCameraUp),
                   cameraRight_(CameraParameters::defaultCameraRight) {
    setDirection(0,0);
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
    return direction_;
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

void Camera::setDirection(float dPitch, float dYam) {
    pitch_ += dPitch * CameraParameters::sensitivity;
    yam_ += dYam * CameraParameters::sensitivity;
    float alfa = pitch_ * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix33 rx(1, 0, 0,
                    0, cosf(alfa), -sinf(alfa),
                    0, sinf(alfa), cosf(alfa));

    float beta = yam_ * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix33 rz(cosf(beta), -sinf(beta), 0,
                    sinf(beta), cosf(beta), 0,
                    0, 0, 1);

    cameraDirection_ = rz * rx * CameraParameters::defaultCameraDirection;
    cameraUp_ = rz * rx * CameraParameters::defaultCameraUp;
    cameraRight_ = rz * rx * CameraParameters::defaultCameraRight;
    direction_ = cameraDirection_ * -1;
}

Vector Camera::getPointAlongDirection(float length) {
    return position_ + direction_ * length;
}
