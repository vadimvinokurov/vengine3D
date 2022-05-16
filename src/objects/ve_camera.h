//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_CAMERA_H
#define VENGINE3D_VE_CAMERA_H

#include "math/ve_vector.h"
#include "math/ve_matrix4.h"
#include "math/ve_transform.h"

namespace VE {
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
		Transform transform;

		Vector3 direction_ = Vector3(0,-1,0);
		Vector3 up_ = Vector3(0,0,1);
		Vector3 right_ = Vector3(1,0,0);
		const float sensitivity = 50.0f;
    };

    using CameraPtr = std::shared_ptr<VE::Camera>;
}


#endif //VENGINE3D_VE_CAMERA_H
