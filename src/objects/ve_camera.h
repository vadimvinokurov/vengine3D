//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "ve_stdlibs.h"
#include "math/ve_vector.h"
#include "math/ve_matrix4.h"

namespace VE {
    namespace CameraParameters {
        const Vector defaultCameraPosition = Vector(7.51278f, -4.72388f, 3.8431);
        const Vector defaultCameraDirection = Vector(0, -1, 0);
        const Vector defaultCameraUp = Vector(0, 0, 1);
        const Vector defaultCameraRight = Vector(1, 0, 0);

        const float sensitivity = 50.0f;
    }

    class Camera {
    public:
        Camera();
        void moveAlongDirection(float distance);
        void moveAlongSide(float distance);

        void setDirection(float dPitch, float dYam);

        Vector getPointAlongDirection(float length);

        const Vector &position() const;
        const Vector &direction() const;
        Matrix4 getViewMatrix() const;

        static Matrix4 perspective(float fov, float aspect, float n, float f);

        static Matrix4 frustum(float left, float right, float bottom, float top, float n, float f);

        static Matrix4 ortho(float left, float right, float bottom, float top, float n, float f);
    private:

        Vector position_;
        Vector direction_;
        Vector cameraDirection_;
        Vector cameraUp_;
        Vector cameraRight_;
        float pitch_ = -15.5557f;
        float yam_ = 51.3889f;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
