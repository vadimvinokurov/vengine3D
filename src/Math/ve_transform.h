//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"
#include "ve_matrix33.h"
#include "stdlibraries.h"

namespace VE {
    class Transform {
    public:
        Transform() : scale(1.0f),
                      rotation(0, 0, 0),
                      position(0, 0, 0) {
        }

        Vector apply(const Vector &localPoint) const {
            return rotateMatrix() * (localPoint * scale) + position;
        }

        Vector applyInverse(const Vector &localPoint) const {
            return rotateMatrix().getTranspose() * (localPoint - position) / scale;
        }

        float scale;
        Vector rotation;
        Vector position;

    private:
        Matrix33 rotateMatrix() const {
            Vector d = rotation.normolize();
            float angle = rotation.abs();
            float cosfi = cosf(angle);
            float sinfi = sinf(angle);

            return Matrix33(
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
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
