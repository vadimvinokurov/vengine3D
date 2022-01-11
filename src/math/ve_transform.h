//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"
#include "ve_matrix33.h"
#include "ve_quaternions.h"
#include "stdlibraries.h"

namespace VE {
    class Transform {
    public:
        Transform(const Vector &_position = Vector(),
                  const Quaternion &_rotation = Quaternion::fromAxisAngle(Vector(0, 1, 0), 0.0f)) :
                rotation(_rotation),
                position(_position) {}

        Vector apply(const Vector &localPoint) const {
            Vector rotatedVector = (rotation * Quaternion(localPoint) * rotation.inverse()).v();
            return rotatedVector + position;
        }

        Vector applyForNormal(const Vector &localNormal) const {
            return (rotation * Quaternion(localNormal) * rotation.inverse()).v();
        }

        Vector applyInverse(const Vector &globalPoint) const {
            Quaternion inverseRotate = rotation.conjugate();
            Vector localPoint = globalPoint - position;
            Vector rv = (inverseRotate * Quaternion(localPoint) * inverseRotate.inverse()).v();
            return rv;
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
