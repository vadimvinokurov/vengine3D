//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"
#include "ve_matrix3.h"
#include "ve_quaternion.h"
#include "stdlibraries.h"

namespace VE {
    class Transform {
    public:
        Transform(const Vector &_position = Vector(),
                  const Quaternion &_rotation = Quaternion::fromAxisAngle(Vector(0, 1, 0), 0.0f)) :
                rotation(_rotation),
                position(_position) {}

        Vector apply(const Vector &localPoint) const {
            return rotation.rotate(localPoint) + position;
        }

        Vector applyForNormal(const Vector &localNormal) const {
            return (rotation * Quaternion(localNormal) * rotation.inverse()).v();
        }

        Vector applyInverse(const Vector &globalPoint) const {
            Quaternion inverseRotate = rotation.conjugate();
            Vector localPoint = globalPoint - position;
            return inverseRotate.rotate(localPoint);
        }

        Vector applyInverseForNormal(const Vector &globalNormal) const {
            Quaternion inverseRotate = rotation.conjugate();
            return (inverseRotate * Quaternion(globalNormal) * inverseRotate.inverse()).v();
        }

        Quaternion rotation;
        Vector position;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
