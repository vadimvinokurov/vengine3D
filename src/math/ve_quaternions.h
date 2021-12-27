//
// Created by boris on 12/27/2021.
//

#ifndef VENGINE3D_VE_QUATERNIONS_H
#define VENGINE3D_VE_QUATERNIONS_H

#include "ve_vector.h"

namespace VE {

    class Quaternion {
    public:
        Quaternion() {}

        Quaternion(const Vector &v, float w) : v_(v), w_(w) {}

        Quaternion(const Vector &v) : Quaternion(v, 0.0f) {}

        Quaternion(const std::pair<Vector, float> &axisAngle) : Quaternion(axisAngle.first * sinf(0.5f * axisAngle.second),
                                                                           cosf(0.5f * axisAngle.second)) {}

        float norma() const {
            return v_.x() * v_.x() + v_.y() * v_.y() + v_.z() * v_.z() + w_ * w_;
        }

        float abs() const {
            return sqrtf(norma());
        }

        Quaternion conjugate() const {
            return Quaternion(v_ * -1, w_);
        }

        Quaternion inverse() const {
            return conjugate() / norma();
        }

        Quaternion operator+(const Quaternion &qb) const {
            return Quaternion(v_ + qb.v_, w_ + qb.w_);
        }

        Quaternion operator-(const Quaternion &qb) const {
            return Quaternion(v_ - qb.v_, w_ - qb.w_);
        }

        Quaternion operator/(float d) const {
            return Quaternion(v_ / d, w_ / d);
        }

        Quaternion operator*(const Quaternion &qb) const {
            Vector vc(v_ *qb.v_ + qb.v_ * w_ + v_ * qb.w_);
            float wc = w_ * qb.w_ - v_.dot(qb.v_);
            return Quaternion(vc, wc);
        }

        Quaternion operator*(const Vector &v) const {
            Quaternion qb(v, 0);
            return (*this) * qb;
        }
        Vector getVector() const {
            return v_;
        }

    private:
        Vector v_;
        float w_ = 0.0f;
    };
}


#endif //VENGINE3D_VE_QUATERNIONS_H
