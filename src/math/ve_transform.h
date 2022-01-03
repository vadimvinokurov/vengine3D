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
        Transform() : scale(1.0f),
                      rotation(Quaternion::fromAxisAngle(Vector(0, 1, 0), 0.0f)),
                      position(0, 0, 0) {
        }


        Vector apply(const Vector &localPoint) const {
            Vector scalePoint = localPoint * scale;
            Vector rotatedVector = (rotation * Quaternion(scalePoint) * rotation.inverse()).v();
            return rotatedVector + position;
        }

        Vector applyForNormal(const Vector &localNormal) const {
            return (rotation * Quaternion(localNormal) * rotation.inverse()).v();
        }

        Vector applyInverse(const Vector &globalPoint) const {
            Quaternion inverseRotate = rotation.conjugate();
            Vector localPoint = globalPoint - position;
            Vector rv = (inverseRotate * Quaternion(localPoint) * inverseRotate.inverse()).v();
            return rv / scale;
        }

        Vector applyInverseForNormal(const Vector &globalNormal) const {
            Quaternion inverseRotate = rotation.conjugate();
            return (inverseRotate * Quaternion(globalNormal) * inverseRotate.inverse()).v();
        }

        float scale;
        Quaternion rotation;
        Vector position;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
