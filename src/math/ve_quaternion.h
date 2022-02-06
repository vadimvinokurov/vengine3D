//
// Created by boris on 12/27/2021.
//

#ifndef VENGINE3D_VE_QUATERNION_H
#define VENGINE3D_VE_QUATERNION_H

#include "ve_vector.h"
#include "ve_matrix4.h"

namespace VE {

    class Quaternion {
        static constexpr float EPSILON = 0.000001f;
    public:
        Quaternion() : v_(0.0f), w_(1.0f) {}

        Quaternion(const Vector &v, float w) : v_(v), w_(w) {}

        Quaternion(float w, const Vector &v) : v_(v), w_(w) {}

        Quaternion(float i, float j, float k, float w) : v_(i, j, k), w_(w) {}

        Quaternion(const Vector &v) : Quaternion(v, 0.0f) {}

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
            float invD = 1.0f / d;
            return Quaternion(v_ * invD, w_ * invD);
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

        bool operator==(const Quaternion &b) {
            return fabsf(this->w_ - b.w_) < EPSILON && this->v_ == b.v_;
        }

        bool operator!=(const Quaternion &b) {
            return !(*this == b);
        }

        float norma() const {
            return (*this).dot(*this);
        }

        float lenSqrt() const {
            return v_.lenSqrt() + w_ * w_;
        }

        float len() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return 0.0f;
            return sqrtf(norma());
        }

        Quaternion getNormalized() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;

            return (*this) / sqrtf(lenSq);
        }

        Quaternion &normalize() {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;
            (*this) /= sqrtf(lenSq);

            return *this;
        }

        Quaternion conjugate() const {
            return Quaternion(v_ * -1, w_);
        }

        Quaternion inverse() const {
            return conjugate() / norma();
        }

        Vector toAxisAngle() const {
            return (v_ / sqrt(1 - w_ * w_)) * (2.0 * acosf(w_));
        }

        Vector getAxis() const {
            return v_.getNormalized();
        }

        float getAngle() const {
            return 2.0 * acosf(w_);
        }

        const Vector &v() const {
            return v_;
        }

        float w() const {
            return w_;
        }

        Vector rotate(const Vector &v) const {
            return (*this * Quaternion(v) * this->inverse()).v();
        }

        Matrix4 toMatrix4() const {
            this->print();
            Vector r = this->rotate(Vector(1, 0, 0));
            Vector f = this->rotate(Vector(0, 1, 0));
            Vector u = this->rotate(Vector(0, 0, 1));

            return Matrix4(r.x, r.y, r.z, 0.0f,
                           u.x, u.y, u.z, 0.0f,
                           f.x, f.y, f.z, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
        }

        static Quaternion fromAxisAngle(const Vector &axisAngle) {
            auto[n, angle] = axisAngle.getNormalAndLen();
            return fromAxisAngle(n, angle);
        }

        static Quaternion fromAxisAngle(const Vector &n, float angle) {
            return Quaternion(n * sinf(0.5f * angle),
                              cosf(0.5f * angle));
        }

        static Quaternion fromTo(const Vector &from, const Vector &to) {
            Vector f = from.getNormalized();
            Vector t = to.getNormalized();

            if (f == t) {
                return Quaternion();
            } else if (f == t * -1.0f) {
                Vector ortho(1, 0, 0);
                if (fabsf(f.y) < fabsf(f.x)) {
                    ortho = Vector(0, 1, 0);
                }
                if (fabsf(f.z) < fabsf(f.y) && fabsf(f.z) < fabsf(f.x)) {
                    ortho = Vector(0, 0, 1);
                }
                return Quaternion((f * ortho).getNormalized(), 0.0f);
            }

            Vector half = (f + t).getNormalized();
            Vector axis = f * half;
            return Quaternion(axis, f.dot(half));

        }

        static Quaternion mix(const Quaternion &from, const Quaternion &to, float t) {
            return from * (1.0f - t) + to * t;
        }

        static Quaternion nlerp(const Quaternion &from, const Quaternion &to, float t) {
            return (from + (to - from) * t).getNormalized();
        }

        static Quaternion lookRotation(const Vector &direction, const Vector &up) {
            Vector f = direction.getNormalized();
            Vector u = up.getNormalized();
            Vector r = u * f;
            u = f * r;

            Quaternion worldToObject = fromTo(Vector(0, 1, 0), f);
            Vector objectUp = worldToObject.rotate(Vector(0, 0, 1));
            Quaternion u2u = fromTo(objectUp, u);

            return (worldToObject * u2u).getNormalized();
        }

        static Quaternion fromMatrix(const Matrix4 &m) {
            auto up = Vector(m.up.x, m.up.y, m.up.z).getNormalized();
            auto forward = Vector(m.forward.x, m.forward.y, m.forward.z).getNormalized();
            Vector right = up * forward;
            up = forward * right;

            return lookRotation(forward, up);
        }

        void print() const {
            std::cout << w_ << " ";
            v_.print();
        }

    private:
        Vector v_;
        float w_ = 0.0f;
    };
}


#endif //VENGINE3D_VE_QUATERNION_H
