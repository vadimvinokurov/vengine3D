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

        Quaternion(float w, const Vector &v) : v_(v), w_(w) {}

        Quaternion(float w, float i, float j, float k) : v_(i, j, k), w_(w) {}

        Quaternion(const Vector &v) : Quaternion(v, 0.0f) {}

        static Quaternion fromAxisAngle(const Vector &axisAngle) {
            auto[n, angle] = axisAngle.getNormalAndLen();
            return fromAxisAngle(n, angle);
        }

        static Quaternion fromAxisAngle(const Vector &n, float angle) {
            return Quaternion(n * sinf(0.5f * angle),
                              cosf(0.5f * angle));
        }

        Vector toAxisAngle() const {
            return (v_ / sqrt(1 - w_ * w_)) * (2 * acos(w_));
        }

        float dot(const Quaternion &b) const {
            return v_.dot(b.v_) + w_ * b.w_;
        }

        Quaternion operator+(const Quaternion &qb) const {
            return Quaternion(v_ + qb.v_, w_ + qb.w_);
        }

        Quaternion operator-(const Quaternion &qb) const {
            return Quaternion(v_ - qb.v_, w_ - qb.w_);
        }

        Quaternion operator*(const Quaternion &b) const {
            Vector vc(v_ *b.v_ + b.v_ * w_ + v_ * b.w_);
            float wc = w_ * b.w_ - v_.dot(b.v_);
            return Quaternion(vc, wc);
        }

        Quaternion operator/(const Quaternion &b) const {
            return (*this) * b.inverse();
        }

        Quaternion operator*(float d) const {
            return Quaternion(v_ * d, w_ * d);
        }

        Quaternion operator/(float d) const {
            return Quaternion(v_ / d, w_ / d);
        }

        Quaternion &operator+=(const Quaternion &qb) {
            *this = *this + qb;
            return *this;
        }

        Quaternion &operator-=(const Quaternion &qb) {
            *this = *this - qb;
            return *this;
        }

        Quaternion &operator*=(float d) {
            *this = *this * d;
            return *this;
        }

        Quaternion &operator/=(float d) {
            *this = *this / d;
            return *this;
        }

        Quaternion &operator*=(const Quaternion &qb) {
            *this = *this * qb;
            return *this;
        }

        Quaternion &operator/=(const Quaternion &qb) {
            *this = *this / qb;
            return *this;
        }

        float norma() const {
            return (*this).dot(*this);
        }

        float abs() const {
            return sqrtf(norma());
        }

        Quaternion normalization() const {
            return (*this) / (*this).abs();
        }

        Quaternion conjugate() const {
            return Quaternion(v_ * -1, w_);
        }

        Quaternion inverse() const {
            return conjugate() / norma();
        }

        const Vector &v() const {
            return v_;
        }

        float w() const {
            return w_;
        }

        Vector rotate(const Vector& v){
            return (*this * Quaternion(v) * this->inverse()).v();
        }

        Matrix33 getMatrix() const {
            float x = v_.x();
            float y = v_.y();
            float z = v_.z();

            float sqrtX = v_.x() * v_.x();
            float sqrtY = v_.y() * v_.y();
            float sqrtZ = v_.z() * v_.z();

            return Matrix33(1 - (2 * sqrtY + 2 * sqrtZ), 2 * x * y + 2 * z * w_, 2 * x * z - 2 * y * w_,
                            2 * x * y - 2 * z * w_, 1 - (2 * sqrtX + 2 * sqrtZ), 2 * y * z + 2 * x * w_,
                            2 * x * z + 2 * y * w_, 2 * y * z - 2 * x * w_, 1 - (2 * sqrtX + 2 * sqrtY));
        }

        void print() {
            std::cout << w_ << " ";
            v_.print();
        }

    private:
        Vector v_;
        float w_ = 0.0f;
    };

    inline Quaternion cross(const Quaternion &P, const Quaternion &Q, const Quaternion &R) {
        float w = (P.v() * Q.v()).dot(R.v());
        Vector v = -P.w() * (Q.v() * R.v()) - Q.w() * (R.v() * P.v()) - R.w() * (P.v() * Q.v());

        return Quaternion(v, w);
    }
}


#endif //VENGINE3D_VE_QUATERNIONS_H
