//
// Created by boris on 11/18/2021.
//

#include "ve_camera.h"
#include "math/ve_matrix3.h"

using namespace VE;

Camera::Camera() {
	transform.position = Vector3(-0.37, 15.0f, 5.5f);
    setDirection(0, 90);
}

void Camera::moveAlongDirection(float distance) {
    transform.position += direction_ * distance * -1;
}

void Camera::moveAlongSide(float distance) {
	transform.position += right_ * distance;
}

const Vector3 &Camera::position() const {
    return transform.position;
}

const Vector3 &Camera::direction() const {
    return direction_;
}

Matrix4 Camera::getViewMatrix() const {
    const Vector3 &R = right_;
    const Vector3 &U = up_;
    const Vector3 &D = direction_;
    const Vector3 &P = transform.position;

    return Matrix4(R.x, R.y, R.z, 0,
                   U.x, U.y, U.z, 0,
                   D.x, D.y, D.z, 0,
                   P.x, P.y, P.z, 1).getInversed();
}

void Camera::setDirection(float dPitch, float dYam) {
	auto rx = Quaternion::fromAxisAngle(right_, dPitch);
	auto rz = Quaternion::fromAxisAngle(Vector3(0,0,1), dYam);
	auto tinv = transform.getInversed();

	transform.rotation = rz * rx * transform.rotation;
	transform.rotation.normalize();
	auto t = transform * tinv;

	direction_ = t.applyToVector(direction_);
	up_ = t.applyToVector(up_);
	right_ = t.applyToVector(right_);
}

Vector3 Camera::getPointAlongDirection(float length) {
    return transform.position + direction_ * -1 * length;
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