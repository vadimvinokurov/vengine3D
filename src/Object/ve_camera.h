//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "stdlibraries.h"
#include "Math/ve_vector.hpp"

namespace VE {
    class Camera final {
    public:
        Camera();
        void setPosition(const Vector &position);
        void setRotation(const Vector &rotation);

        const Vector& position();
        const Vector& rotation();
        ~Camera();
    private:
        Vector position_;
        Vector rotation_;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
