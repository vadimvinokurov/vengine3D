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
    struct Vector {
        static constexpr float EPSILON = 0.000001f;

        Vector(float fillValue = 0.0f) : x(fillValue), y(fillValue), z(fillValue) {};

        Vector(float x_, float y_, float z_ = 0.0f) : x(x_), y(y_), z(z_) {};

        Vector(float *fv) : x(fv[0]), y(fv[1]), z(fv[2]) {};

        Vector(const Vector &vector) = default;

        Vector &operator=(const Vector &vector) = default;

        Vector operator+(const Vector &other) const {
            return Vector(x + other.x, y + other.y, z + other.z);
        }

        Vector operator-(const Vector &other) const {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        Vector operator*(float factor) const {
            return Vector(x * factor, y * factor, z * factor);
        }

        Vector operator/(float factor) const {
            float invFactor = 1.0f / factor;
            return Vector(x * invFactor, y * invFactor, z * invFactor);
        }

        Vector operator*(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
        }

        Vector &operator+=(const Vector &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector &operator-=(const Vector &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector &operator*=(const Vector &other) {
            const Vector &a = (*this);
            const Vector &b = other;

            float tmpx_ = a.y * b.z - a.z * b.y;
            float tmpy_ = a.z * b.x - a.x * b.z;
            float tmpz_ = a.x * b.y - a.y * b.x;

            x = tmpx_;
            y = tmpy_;
            z = tmpz_;

            return *this;
        }

        Vector &operator*=(float factor) {
            x *= factor;
            y *= factor;
            z *= factor;

            return *this;
        }

        Vector &operator/=(float factor) {
            float invFactor = 1.0f / factor;

            x *= invFactor;
            y *= invFactor;
            z *= invFactor;

            return *this;
        }

        float dot(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
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

        Vector getNormalized() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;
            return (*this) / sqrtf(lenSq);
        }

        Vector &normalize() {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return *this;
            (*this) /= sqrtf(lenSq);
            return *this;
        }

        std::pair<Vector, float> getNormalAndLen() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) {
                return {*this, 0.0f};
            }
            float magnitude = sqrtf(lenSq);
            return {(*this) / magnitude, magnitude};
        }

        bool operator==(const Vector &other) const {
            return (*this - other).lenSqrt() < EPSILON;
        }

        bool operator!=(const Vector &other) const {
            return !(*this == other);
        }

        Vector getProjection(const VE::Vector &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector();
            float scale = this->dot(b) / bLenSqrt;
            return b * scale;
        }

        Vector getRejection(const VE::Vector &b) const {
            return *this - this->getProjection(b);
        }

        Vector getReflection(const VE::Vector &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector();
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

        void draw(const Vector &basePoint = Vector(), const VE::Color &color = VE::Color()) const {
            glLineWidth(2);
            glColor3f(color.red(), color.grean(), color.blue());
            glBegin(GL_LINES);
            glVertex3f(basePoint.x, basePoint.y, basePoint.z);
            Vector vector = basePoint + *this;
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
            draw(VE::Vector(), color);
        }

        void print() const {
            std::cout << x << " " << y << " " << z << std::endl;
        }

        static float angle(const Vector &a, const Vector &b) {
            float aLenSqrt = a.lenSqrt();
            float bLenSqrt = b.lenSqrt();
            if (aLenSqrt < EPSILON || bLenSqrt < EPSILON) {
                return 0.0f;
            }
            return acosf(a.dot(b) / (sqrtf(aLenSqrt) * sqrtf(bLenSqrt)));
        }

        static Vector lerp(const Vector &s, const Vector &e, float t) {
            //return (1 - t) * s + t * e;
            return s + (e - s) * t;
        }

        static Vector nlerp(const Vector &s, const Vector &e, float t) {
            return (s + (e - s) * t).getNormalized();
        }

        static Vector slerp(const Vector &s, const Vector &e, float t) {
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

    inline Vector operator*(float factor, const Vector &v) {
        return v * factor;
    }

    inline bool sameDirection(const Vector &a, const Vector &b) {
        return a.dot(b) > 0.0f;
    };
}


#endif //VOLLEYBALL_VECTOR_HPP
