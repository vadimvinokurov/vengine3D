//
// Created by boris on 12/27/2021.
//

#ifndef VENGINE3D_VE_QUATERNION_H
#define VENGINE3D_VE_QUATERNION_H

#include "ve_engine_settings.h"
#include "ve_vector.h"
#include "ve_matrix4.h"

namespace VE {

    struct Quaternion {
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

        Quaternion(float fillValue) : x(fillValue), y(fillValue), z(fillValue), w(fillValue) {}

        Quaternion(const Vector3 &v, float w_ = 0.0f) : x(v.x), y(v.y), z(v.z), w(w_) {}

        Quaternion(const Vector4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

        Quaternion(const float *fv) : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) {}

        Quaternion operator+(const Quaternion &other) const {
            return Quaternion(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
        }

        Quaternion operator-(const Quaternion &other) const {
            return Quaternion(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
        }

        Quaternion operator*(float f) const {
            return Quaternion(x * f, y * f, z * f, w * f);
        }

        Quaternion operator/(float f) const {
            float invf = 1.0f / f;
            return Quaternion(x * invf, y * invf, z * invf, w * invf);
        }

        Quaternion operator*(const Quaternion &other) const {
            return Quaternion(
                    this->x * other.w + this->y * other.z - this->z * other.y + this->w * other.x,
                    -this->x * other.z + this->y * other.w + this->z * other.x + this->w * other.y,
                    this->x * other.y - this->y * other.x + this->z * other.w + this->w * other.z,
                    -this->x * other.x - this->y * other.y - this->z * other.z + this->w * other.w
            );
        }

        Quaternion operator/(const Quaternion &b) const {
            return (*this) * b.inverse();
        }

        Quaternion &operator+=(const Quaternion &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Quaternion &operator-=(const Quaternion &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Quaternion &operator*=(float f) {
            x *= f;
            y *= f;
            z *= f;
            w *= f;
            return *this;
        }

        Quaternion &operator/=(float f) {
            float invf = 1.0f / f;
            x *= invf;
            y *= invf;
            z *= invf;
            w *= invf;
            return *this;
        }

        Quaternion &operator*=(const Quaternion &other) {
            *this = *this * other;
            return *this;
        }

        Quaternion &operator/=(const Quaternion &other) {
            *this = *this / other;
            return *this;
        }

        bool operator==(const Quaternion &other) const {
            return (fabsf(this->x - other.x) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->y - other.y) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->z - other.z) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->w - other.w) < VEngineSettings::QUATERNION_EPSILON);
        }

        bool operator!=(const Quaternion &other) const {
            return !(*this == other);
        }

        bool sameOrientation(const Quaternion &other) const {
            return (fabsf(this->x - other.x) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->y - other.y) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->z - other.z) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->w - other.w) < VEngineSettings::QUATERNION_EPSILON) ||
                   (fabsf(this->x + other.x) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->y + other.y) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->z + other.z) < VEngineSettings::QUATERNION_EPSILON &&
                    fabsf(this->w + other.w) < VEngineSettings::QUATERNION_EPSILON);
        }

        float dot(const Quaternion &other) const {
            return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
        }

        float norma() const {
            return (*this).dot(*this);
        }

        float lenSqrt() const {
            return x * x + y * y + z * z + w * w;
        }

        float len() const {
            float lenSq = lenSqrt();
            if (lenSq < VEngineSettings::QUATERNION_EPSILON) return 0.0f;
            return sqrtf(norma());
        }

        Quaternion getNormalized() const {
            float lenSq = lenSqrt();
            if (lenSq < VEngineSettings::QUATERNION_EPSILON) return *this;

            return (*this) / sqrtf(lenSq);
        }

        Quaternion &normalize() {
            float lenSq = lenSqrt();
            if (lenSq < VEngineSettings::QUATERNION_EPSILON) return *this;
            (*this) /= sqrtf(lenSq);

            return *this;
        }

        Quaternion conjugate() const {
            return Quaternion(x * -1.0f, y * -1.0f, z * -1.0f, w);
        }

        Quaternion inverse() const {
            return conjugate() / norma();
        }

        Vector3 getAxis() const {
            return Vector3(x, y, z).getNormalized();
        }

        float getAngle() const {
            return 2.0 * acosf(w);
        }

        Vector3 rotate(const Vector3 &v) const {
            Quaternion tmp = *this * Quaternion(v) * this->inverse();
            return Vector3(tmp.x, tmp.y, tmp.z);
        }

        Matrix4 toMatrix4() const {
            Vector3 nx = this->rotate(Vector3(1, 0, 0));
            Vector3 ny = this->rotate(Vector3(0, 1, 0));
            Vector3 nz = this->rotate(Vector3(0, 0, 1));

            return Matrix4(nx.x, nx.y, nx.z, 0.0f,
						   ny.x, ny.y, ny.z, 0.0f,
						   nz.x, nz.y, nz.z, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
        }

        static Quaternion fromAxisAngle(const Vector3 &axisAngle) {
            auto[axis, angle] = axisAngle.getNormalAndLen();
            return fromAxisAngle(axis, angle);
        }

        static Quaternion fromAxisAngle(const Vector3 &axis, float angle) {
            return Quaternion(axis * sinf(0.5f * angle),
                              cosf(0.5f * angle));
        }

        static Quaternion fromTo(const Vector3 &from, const Vector3 &to) {
            Vector3 f = from.getNormalized();
            Vector3 t = to.getNormalized();

            if (f == t) {
                return Quaternion();
            } else if (f == t * -1.0f) {
                Vector3 ortho(1, 0, 0);
                if (fabsf(f.y) < fabsf(f.x)) {
                    ortho = Vector3(0, 1, 0);
                }
                if (fabsf(f.z) < fabsf(f.y) && fabsf(f.z) < fabsf(f.x)) {
                    ortho = Vector3(0, 0, 1);
                }
                return Quaternion((f * ortho).getNormalized(), 0.0f);
            }

            Vector3 half = (f + t).getNormalized();
            Vector3 axis = f * half;
            return Quaternion(axis, f.dot(half));

        }

        static Quaternion mix(const Quaternion &from, const Quaternion &to, float t) {
            return from * (1.0f - t) + to * t;
        }

        static Quaternion nlerp(const Quaternion &from, const Quaternion &to, float t) {
            return (from + (to - from) * t).getNormalized();
        }

        static Quaternion lookRotation(const Vector3 &forward, const Vector3 &up) {
            Vector3 f = forward.getNormalized();
            Vector3 u = up.getNormalized();
            Vector3 r = f * u;
            u = r * f ;

            Quaternion worldToObject = fromTo(Vector3(0, 1, 0), f);
            Vector3 objectUp = worldToObject.rotate(Vector3(0, 0, 1));
            Quaternion u2u = fromTo(objectUp, u);

            return (u2u * worldToObject).getNormalized();
        }

        static Quaternion fromMatrix(const Matrix4 &m) {
            auto up = Vector3(m.modelUp.x, m.modelUp.y, m.modelUp.z).getNormalized();
            auto forward = Vector3(m.modelForward.x, m.modelForward.y, m.modelForward.z).getNormalized();
            Vector3 right = forward * up;
            up = right * forward;

            return lookRotation(forward, up);
        }

        void print() const {
            std::cout << x << " " << y << " " << z << " " << w << std::endl;
        }

        const float *data() const {
            return v;
        }

        float *data() {
            return v;
        }

        static constexpr std::size_t size(){
            return 4;
        }

        union {
            float v[4];
            struct {
                float x;
                float y;
                float z;
                float w;
            };
        };
    };
}


#endif //VENGINE3D_VE_QUATERNION_H
