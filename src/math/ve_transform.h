//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"
#include "ve_matrix3.h"
#include "ve_quaternion.h"
#include "ve_stdlibs.h"

namespace VE {
    struct Transform {
        static constexpr float EPSILON = 0.000001f;

        Transform() : position(0, 0, 0),
                      rotation(0, 0, 0, 1),
                      scale(1, 1, 1) {}

        Transform(const Vector3 &_position, const Quaternion &_rotation, const Vector3 &_scale) :
                position(_position),
                rotation(_rotation),
                scale(_scale) {}

        Transform(const Vector3 &_position) :
                position(_position),
                rotation(0, 0, 0, 1),
                scale(1, 1, 1) {}

        Transform(const Quaternion &_rotation) :
                position(0, 0, 0),
                rotation(_rotation),
                scale(1, 1, 1) {}

        Vector3 applyToPoint(const Vector3 &localPoint) const {
            return rotation.rotate(multiply(localPoint, scale)) + position;
        }

        Vector3 applyToVector(const Vector3 &localPoint) const {
            return rotation.rotate(multiply(localPoint, scale));
        }

        Transform getInversed() {
            Transform inv;

            inv.rotation = this->rotation.inverse();

            inv.scale.x = fabsf(scale.x) < EPSILON ? 0.0f : 1.0f / scale.x;
            inv.scale.y = fabsf(scale.y) < EPSILON ? 0.0f : 1.0f / scale.y;
            inv.scale.z = fabsf(scale.z) < EPSILON ? 0.0f : 1.0f / scale.z;

            Vector3 invTrans = position * -1.0f;

            inv.position = inv.rotation.rotate(multiply(inv.scale, invTrans));

            return inv;
        }

        Matrix4 toMatrix() const {
            Vector3 x = rotation.rotate(Vector3(1, 0, 0));
            Vector3 y = rotation.rotate(Vector3(0, 1, 0));
            Vector3 z = rotation.rotate(Vector3(0, 0, 1));

            x *= scale.x;
            y *= scale.y;
            z *= scale.z;

            return Matrix4(
                    x.x, x.y, x.z, 0,
                    y.x, y.y, y.z, 0,
                    z.x, z.y, z.z, 0,
                    position.x, position.y, position.z, 1
            );

        }

        static Transform fromMatrix(const Matrix4 &m) {
            Transform out;

            out.position = Vector3(m.v[12], m.v[13], m.v[14]);
            out.rotation = Quaternion::fromMatrix(m);

            Matrix4 rotScaleMatrix(
                    m.v[0], m.v[1], m.v[2], 0,
                    m.v[4], m.v[5], m.v[6], 0,
                    m.v[8], m.v[9], m.v[10], 0,
                    0, 0, 0, 1
            );

            Matrix4 invRotationMatrix = out.rotation.inverse().toMatrix4();
            Matrix4 scaleSkewMatrix = rotScaleMatrix * invRotationMatrix;

            out.scale.x = scaleSkewMatrix.v[0];
            out.scale.y = scaleSkewMatrix.v[5];
            out.scale.z = scaleSkewMatrix.v[10];

            return out;
        }

        static Transform combine(const Transform &a, const Transform &b) {
            Transform out;
            out.scale = multiply(a.scale, b.scale);
            out.rotation = b.rotation * a.rotation;

            out.position = a.rotation.rotate(multiply(a.scale, b.position));
            out.position += a.position;

            return out;
        }

        static Transform mix(const Transform &a, const Transform &b, float t) {
            Quaternion bRotation = b.rotation;
            if (a.rotation.dot(b.rotation) < 0.0f) {
                bRotation *= -1;
            }

            return Transform(
                    Vector3::lerp(a.position, b.position, t),
                    Quaternion::nlerp(a.rotation, bRotation, t),
                    Vector3::lerp(a.scale, b.scale, t)
            );
        }


        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
