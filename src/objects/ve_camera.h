//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "math/ve_vector.h"
#include "math/ve_matrix4.h"

namespace VE {
    namespace CameraParameters {
        const Vector3 defaultCameraPosition = Vector3(-0.37, 15.0f, 6.5f);
        const Vector3 defaultCameraDirection = Vector3(0, -1, 0);
        const Vector3 defaultCameraUp = Vector3(0, 0, 1);
        const Vector3 defaultCameraRight = Vector3(1, 0, 0);

        const float sensitivity = 50.0f;
    }

    class Camera {
    public:
        Camera();
        void moveAlongDirection(float distance);
        void moveAlongSide(float distance);

        void setDirection(float dPitch, float dYam);

        Vector3 getPointAlongDirection(float length);

        const Vector3 &position() const;
        const Vector3 &direction() const;
        Matrix4 getViewMatrix() const;

        static Matrix4 perspective(float fov, float aspect, float n, float f);

        static Matrix4 frustum(float left, float right, float bottom, float top, float n, float f);

        static Matrix4 ortho(float left, float right, float bottom, float top, float n, float f);
    private:

        Vector3 position_;
        Vector3 direction_;
        Vector3 cameraDirection_;
        Vector3 cameraUp_;
        Vector3 cameraRight_;
        float pitch_ = -13.0f;
        float yam_ = 160.0f;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
