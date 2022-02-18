//
// Created by boris on 11/18/2021.
//

#include "ve_camera.h"
#include "math/ve_matrix3.h"

using namespace VE;

Camera::Camera() : position_(CameraParameters::defaultCameraPosition),
                   cameraDirection_(CameraParameters::defaultCameraDirection),
                   cameraUp_(CameraParameters::defaultCameraUp),
                   cameraRight_(CameraParameters::defaultCameraRight) {
    setDirection(0, 0);
}

void Camera::moveAlongDirection(float distance) {
    position_ += cameraDirection_ * distance * -1;
}

void Camera::moveAlongSide(float distance) {
    position_ += cameraRight_ * distance;
}

const Vector3 &Camera::position() const {
    return position_;
}

const Vector3 &Camera::direction() const {
    return direction_;
}

Matrix4 Camera::getViewMatrix() const {
    const Vector3 &R = cameraRight_;
    const Vector3 &U = cameraUp_;
    const Vector3 &D = cameraDirection_;
    const Vector3 &P = position_;

    return Matrix4(R.x, R.y, R.z, 0,
                   U.x, U.y, U.z, 0,
                   D.x, D.y, D.z, 0,
                   P.x, P.y, P.z, 1).getInversed();

//    return Matrix4(cameraRight_.x, cameraUp_.x, cameraDirection_.x, 0,
//                   cameraRight_.y, cameraUp_.y, cameraDirection_.y, 0,
//                   cameraRight_.z, cameraUp_.z, cameraDirection_.z, 0,
//                   -R.dot(P), -U.dot(P), -D.dot(P), 1);
}

void Camera::setDirection(float dPitch, float dYam) {
    pitch_ += dPitch * CameraParameters::sensitivity;
    yam_ += dYam * CameraParameters::sensitivity;
    float alfa = pitch_ * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix3 rx(1, 0, 0,
                   0, cosf(alfa), -sinf(alfa),
                   0, sinf(alfa), cosf(alfa));

    float beta = yam_ * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix3 rz(cosf(beta), -sinf(beta), 0,
                   sinf(beta), cosf(beta), 0,
                   0, 0, 1);

    cameraDirection_ = rz * rx * CameraParameters::defaultCameraDirection;
    cameraUp_ = rz * rx * CameraParameters::defaultCameraUp;
    cameraRight_ = rz * rx * CameraParameters::defaultCameraRight;
    direction_ = cameraDirection_ * -1;
}

Vector3 Camera::getPointAlongDirection(float length) {
    return position_ + direction_ * length;
}

Matrix4 Camera::perspective(float fov, float aspect, float n, float f) {
    float ymax = n * tanf(fov * static_cast<float>(M_PI) / 360.0f);
    float xmax = ymax * aspect;
    return Camera::frustum(-xmax, xmax, -ymax, ymax, n, f);
}

Matrix4 Camera::frustum(float left, float right, float bottom, float top, float n, float f) {
    if (left == right || top == bottom || n == f) {
        assert(false && "Invalid frustum\n");
        return Matrix4();
    }
    return Matrix4(
            (2.0f * n) / (right - left), 0.0f, 0.0f, 0.0f,
            0.0f, (2.0f * n) / (top - bottom), 0.0f, 0.0f,
            (right + left) / (right - left), (top + bottom) / (top - bottom), (-(f + n)) / (f - n), -1,
            0.0f, 0.0f, (-2.0f * f * n) / (f - n), 0.0f);
}

Matrix4 Camera::ortho(float left, float right, float bottom, float top, float n, float f) {
    if (left == right || top == bottom || n == f) {
        assert(false && "Invalid frustum\n");
        return Matrix4();
    }
    return Matrix4(
            2.0f / (right - left), 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
            0.0f, 0.0f, -2.0f / (f - n), 0.0f,
            -(right + left) / (right - left), -(top + bottom) / (top - bottom), (-(f + n)) / (f - n), 1);
}