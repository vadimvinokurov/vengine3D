//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "stdlibraries.h"
#include "math/ve_vector.h"
#include "stdlibraries.h"

namespace VE {
    class Camera final {
    public:
        Camera();
        void moveAlongDirection(float distance);
        void moveAlongSide(float distance);

        void setDirection(float pitch, float yam);

        const Vector &position() const;
        const Vector &direction() const;
        const std::array<float, 16> &getViewMatrix() const;
        ~Camera();
    private:
        mutable std::array<float, 16> viewMatrix_;
        Vector position_;
        Vector cameraDirection_;
        Vector cameraUp_;
        Vector cameraRight_;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
