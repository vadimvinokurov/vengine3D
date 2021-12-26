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

        Vector applyForNormal(const Vector &localNormal) const {
            return rotateMatrix() * localNormal;
        }

        Vector applyInverse(const Vector &globalPoint) const {
            return rotateMatrix().getTranspose() * (globalPoint - position) / scale;
        }

        Vector applyInverseForNormal(const Vector &globalNormal) const {
            return rotateMatrix().getTranspose() * globalNormal;
        }

        float scale;
        Vector rotation;
        Vector position;

    private:
        Matrix33 rotateMatrix() const {
            return rotateMatrixAxisAngle();
        }

        Matrix33 rotateMatrixEuler() const {
            float alfa = rotation.x();
            float beta = rotation.y();
            float gamma = rotation.z();

            VE::Matrix33 rx(1, 0, 0,
                            0, cosf(alfa), -sinf(alfa),
                            0, sinf(alfa), cosf(alfa));

            VE::Matrix33 ry(cosf(beta), 0, sinf(beta),
                            0, 1, 0,
                            -sinf(beta), 0, cosf(beta));

            VE::Matrix33 rz(cosf(gamma), -sinf(gamma), 0,
                            sinf(gamma), cosf(gamma), 0,
                            0, 0, 1);

            return rz * ry * rx;
        }

        Matrix33 rotateMatrixAxisAngle() const {
            Vector d = rotation.normolize();
            float angle = rotation.abs();
            float cosfi = cosf(angle);
            float sinfi = sinf(angle);
            float t = (1 - cosfi);

            return Matrix33(
                    cosfi +  t* d.x() * d.x(),
                    t * d.x() * d.y() - sinfi * d.z(),
                    t * d.x() * d.z() + sinfi * d.y(),

                    t * d.y() * d.x() + sinfi * d.z(),
                    cosfi + t * d.y() * d.y(),
                    t * d.y() * d.z() - sinfi * d.x(),

                    t * d.z() * d.x() - sinfi * d.y(),
                    t * d.z() * d.y() + sinfi * d.x(),
                    cosfi + t * d.z() * d.z()
            );
        }
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
