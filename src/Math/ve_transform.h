//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"
#include "ve_matrix33.h"

namespace VE {
    class Transform {
    public:
        Transform() : scale(1, 1, 1),
                      rotation(0, 0, 0),
                      position(0, 0, 0) {
        }

        Vector applyTransform(const Vector &localPoint) {
            setRotateMatrix();
            return rotateMatrix * scalePoint(localPoint) + position;
        }

        Vector scale;
        Vector rotation;
        Vector position;

    private:
        Matrix33 rotateMatrix;

        void setRotateMatrix() {
            Vector d = rotation.normolize();
            float angle = rotation.abs();
            float cosfi = cosf(angle);
            float sinfi = sinf(angle);

            rotateMatrix = Matrix33(
                    cosfi + (1 - cosfi) * d.x() * d.x(),
                    (1 - cosfi) * d.x() * d.y() - sinfi * d.z(),
                    (1 - cosfi) * d.x() * d.z() + sinfi * d.y(),

                    (1 - cosfi) * d.y() * d.x() + sinfi * d.z(),
                    cosfi + (1 - cosfi) * d.y() * d.y(),
                    (1 - cosfi) * d.y() * d.z() - sinfi * d.x(),

                    (1 - cosfi) * d.z() * d.x() - sinfi * d.y(),
                    (1 - cosfi) * d.z() * d.y() + sinfi * d.x(),
                    cosfi + (1 - cosfi) * d.z() * d.z()
            );
        }

        Vector scalePoint(const Vector &scalablePoint) {
            return Vector(scalablePoint.x() * scale.x(), scalablePoint.y() * scale.y(), scalablePoint.z() * scale.z());
        }
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
