//
// Created by Boris on 8/24/2021.
//

#ifndef VOLLEYBALL_VECTOR_HPP
#define VOLLEYBALL_VECTOR_HPP

#include <cmath>
#include <iostream>
#include <GL/gl.h>

#include "ve_color.h"

namespace VE {
    struct Vector3 {
        static constexpr float EPSILON = 0.000001f;

        Vector3(float fillValue = 0.0f) : x(fillValue), y(fillValue), z(fillValue) {};

        Vector3(float x_, float y_, float z_ = 0.0f) : x(x_), y(y_), z(z_) {};

        Vector3(float *fv) : x(fv[0]), y(fv[1]), z(fv[2]) {};

        Vector3(const Color &c) : x(c.red()), y(c.grean()), z(c.blue()) {};

        Vector3(const Vector3 &vector) = default;

        Vector3 &operator=(const Vector3 &vector) = default;

        Vector3 operator+(const Vector3 &other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3 &other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float factor) const {
            return Vector3(x * factor, y * factor, z * factor);
        }

        Vector3 operator/(float factor) const {
            float invFactor = 1.0f / factor;
            return Vector3(x * invFactor, y * invFactor, z * invFactor);
        }

        Vector3 operator*(const Vector3 &other) const {
            const Vector3 &a = (*this);
            const Vector3 &b = other;
            return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
        }

        Vector3 &operator+=(const Vector3 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3 &operator-=(const Vector3 &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3 &operator*=(const Vector3 &other) {
            const Vector3 &a = (*this);
            const Vector3 &b = other;

            float tmpx_ = a.y * b.z - a.z * b.y;
            float tmpy_ = a.z * b.x - a.x * b.z;
            float tmpz_ = a.x * b.y - a.y * b.x;

            x = tmpx_;
            y = tmpy_;
            z = tmpz_;

            return *this;
        }

        Vector3 &operator*=(float factor) {
            x *= factor;
            y *= factor;
            z *= factor;

            return *this;
        }

        Vector3 &operator/=(float factor) {
            float invFactor = 1.0f / factor;

            x *= invFactor;
            y *= invFactor;
            z *= invFactor;

            return *this;
        }

        float dot(const Vector3 &other) const {
            const Vector3 &a = (*this);
            const Vector3 &b = other;
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        float lenSqrt() const {
            return x * x + y * y + z * z;
        }

        float len() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return 0.0f;
            return sqrtf(lenSq);
        }

        Vector3 getNormalized() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;
            return (*this) / sqrtf(lenSq);
        }

        Vector3 &normalize() {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;
            (*this) /= sqrtf(lenSq);
            return *this;
        }

        std::pair<Vector3, float> getNormalAndLen() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) {
                return {*this, 0.0f};
            }
            float magnitude = sqrtf(lenSq);
            return {(*this) / magnitude, magnitude};
        }

        bool operator==(const Vector3 &other) const {
            return (*this - other).lenSqrt() < EPSILON;
        }

        bool operator!=(const Vector3 &other) const {
            return !(*this == other);
        }

        Vector3 getProjection(const VE::Vector3 &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector3();
            float scale = this->dot(b) / bLenSqrt;
            return b * scale;
        }

        Vector3 getRejection(const VE::Vector3 &b) const {
            return *this - this->getProjection(b);
        }

        Vector3 getReflection(const VE::Vector3 &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector3();
            float scale = this->dot(b) / bLenSqrt;
            auto projectionX2 = b * scale * 2;
            return *this - projectionX2;
        }

        void round(float roundSize = 10000.0f) {
            x = static_cast<int>(x * roundSize) / roundSize;
            y = static_cast<int>(y * roundSize) / roundSize;
            z = static_cast<int>(z * roundSize) / roundSize;
        }

        void setZero() {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        };

        const float *data() const {
            return v;
        }

        //Debug function

        void draw(const Vector3 &basePoint = Vector3(), const VE::Color &color = VE::Color()) const {
            glLineWidth(2);
            glColor3f(color.red(), color.grean(), color.blue());
            glBegin(GL_LINES);
            glVertex3f(basePoint.x, basePoint.y, basePoint.z);
            Vector3 vector = basePoint + *this;
            glVertex3f(vector.x, vector.y, vector.z);
            glEnd();
            glPointSize(6);
            glBegin(GL_POINTS);
            glVertex3f(vector.x, vector.y, vector.z);
            glEnd();
        }

        void drawPoint(int size = 6, const VE::Color &color = VE::Color()) const {
            glColor3f(color.red(), color.grean(), color.blue());
            glPointSize(size);
            glBegin(GL_POINTS);
            glVertex3f(this->x, this->y, this->z);
            glEnd();
        }

        void drawPoint(const VE::Color &color) const {
            drawPoint(6, color);
        }

        void draw(const VE::Color &color) const {
            draw(VE::Vector3(), color);
        }

        void print() const {
            std::cout << x << " " << y << " " << z << std::endl;
        }

        static float angle(const Vector3 &a, const Vector3 &b) {
            float aLenSqrt = a.lenSqrt();
            float bLenSqrt = b.lenSqrt();
            if (aLenSqrt < EPSILON || bLenSqrt < EPSILON) {
                return 0.0f;
            }
            return acosf(a.dot(b) / (sqrtf(aLenSqrt) * sqrtf(bLenSqrt)));
        }

        static Vector3 lerp(const Vector3 &s, const Vector3 &e, float t) {
            //return (1 - t) * s + t * e;
            return s + (e - s) * t;
        }

        static Vector3 nlerp(const Vector3 &s, const Vector3 &e, float t) {
            return lerp(s, e, t).getNormalized();
        }

        static Vector3 slerp(const Vector3 &s, const Vector3 &e, float t) {
            if (t < 0.01f) return lerp(s, e, t);

            float theta = angle(s, e);
            float sinTheta = sinf(theta);

            float a = sinf((1.0f - t) * theta) / sinTheta;
            float b = sinf(t * theta) / sinTheta;
            return s * a + e * b;
        }

        union {
            float v[3];
            struct {
                float x = 0.0f;
                float y = 0.0f;
                float z = 0.0f;
            };
        };

    };

    inline Vector3 operator*(float factor, const Vector3 &v) {
        return v * factor;
    }

    inline Vector3 multiply(const Vector3 &a, const Vector3 &b) {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline bool sameDirection(const Vector3 &a, const Vector3 &b) {
        return a.dot(b) > 0.0f;
    };

    template<typename T>
    struct TVector2 {
        TVector2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

        TVector2(T x_, T y_) : x(x_), y(y_) {}

        TVector2(T *fv) : x(fv[0]), y(fv[1]) {}

        const T *data() const {
            return &x;
        }

        union {
            struct {
                T x;
                T y;
            };
            T v[2];
        };
    };

    using Vector2 = TVector2<float>;
    using IVector2 = TVector2<int>;

    template<typename T>
    struct TVector4 {
        TVector4() : x(static_cast<T>(0)),
                     y(static_cast<T>(0)),
                     z(static_cast<T>(0)),
                     w(static_cast<T>(0)) {}

        TVector4(T x_, T y_, T z_, T w_) : x(x_),
                                           y(y_),
                                           z(z_),
                                           w(w_) {}

        TVector4(T *fv) : x(fv[0]),
                          y(fv[1]),
                          z(fv[2]),
                          w(fv[3]) {}

        const T *data() const {
            return &x;
        }

        union {
            struct {
                T x;
                T y;
                T z;
                T w;
            };
            T v[4];
        };
    };

    using Vector4 = TVector4<float>;
    using IVector4 = TVector4<int>;
    using UIVector4 = TVector4<unsigned int>;

    struct SVector4 {
        union {
            struct {
                float x;
                float y;
                float z;
                float w;
            };
            float v[4];
        };
    };
}


#endif //VOLLEYBALL_VECTOR_HPP
