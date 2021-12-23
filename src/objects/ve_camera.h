//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "stdlibraries.h"
#include "math/ve_vector.h"
#include "stdlibraries.h"

namespace VE {
    namespace CameraParameters {
        const Vector defaultCameraDirection = Vector(0, -1, 0);
        const Vector defaultCameraUp = Vector(0, 0, 1);
        const Vector defaultCameraRight = Vector(1, 0, 0);

        const float sensitivity = 50.0f;
    }
    class Camera final {
    public:
        Camera();
        void moveAlongDirection(float distance);
        void moveAlongSide(float distance);

        void setDirection(float dPitch, float dYam);

        Vector getPointAlongDirection(float length);

        const Vector &position() const;
        const Vector &direction() const;
        const std::array<float, 16> &getViewMatrix() const;
        ~Camera();
    private:
        mutable std::array<float, 16> viewMatrix_;
        Vector position_;
        Vector direction_;
        Vector cameraDirection_;
        Vector cameraUp_;
        Vector cameraRight_;
        float pitch_ = 0.0f;
        float yam_ = 0.0f;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
