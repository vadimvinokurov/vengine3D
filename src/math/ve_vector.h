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

        Vector(float fillValue = 0.0f) : x_(fillValue), y_(fillValue), z_(fillValue) {};

        Vector(float x, float y, float z = 0.0f) : x_(x), y_(y), z_(z) {};

        Vector(const Vector &vector) = default;

        Vector &operator=(const Vector &vector) = default;

        Vector operator+(const Vector &other) const {
            return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
        }

        Vector operator-(const Vector &other) const {
            return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
        }

        Vector operator*(float factor) const {
            return Vector(x_ * factor, y_ * factor, z_ * factor);
        }

        Vector operator/(float factor) const {
            float invFactor = 1.0f / factor;
            return (*this) * invFactor;
        }

        Vector operator*(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return Vector(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
        }

        Vector &operator+=(const Vector &other) {
            x_ += other.x_;
            y_ += other.y_;
            z_ += other.z_;
            return *this;
        }

        Vector &operator-=(const Vector &other) {
            x_ -= other.x_;
            y_ -= other.y_;
            z_ -= other.z_;
            return *this;
        }

        Vector &operator*=(const Vector &other) {
            const Vector &a = (*this);
            const Vector &b = other;

            x_ = a.y() * b.z() - a.z() * b.y();
            y_ = a.z() * b.x() - a.x() * b.z();
            z_ = a.x() * b.y() - a.y() * b.x();

            return *this;
        }

        Vector &operator*=(float factor) {
            x_ *= factor;
            y_ *= factor;
            z_ *= factor;

            return *this;
        }

        Vector &operator/=(float factor) {
            float invFactor = 1.0f / factor;

            x_ *= invFactor;
            y_ *= invFactor;
            z_ *= invFactor;

            return *this;
        }

        float dot(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
        }

        float lenSqrt() const {
            return x_ * x_ + y_ * y_ + z_ * z_;
        }

        float len() const {
            float lenSq = lenSqrt();
            if (lenSq < EPSILON) return 0.0f;
            return sqrtf(lenSq);
        }

        Vector normalized() const {
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

        Vector projection(const VE::Vector &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector();
            float scale = this->dot(b) / bLenSqrt;
            return b * scale;
        }

        Vector rejection(const VE::Vector &b) const {
            return *this - this->projection(b);
        }

        Vector reflection(const VE::Vector &b) const {
            float bLenSqrt = b.lenSqrt();

            if (bLenSqrt < EPSILON)
                return Vector();
            float scale = this->dot(b) / bLenSqrt;
            auto projectionX2 = b * scale * 2;
            return *this - projectionX2;
        }

        float x() const { return x_; }

        float y() const { return y_; }

        float z() const { return z_; }

        void setX(float x) { x_ = x; }

        void setY(float y) { y_ = y; }

        void setZ(float z) { z_ = z; }

        void round(float roundSize = 10000.0f) {
            x_ = static_cast<int>(x_ * roundSize) / roundSize;
            y_ = static_cast<int>(y_ * roundSize) / roundSize;
            z_ = static_cast<int>(z_ * roundSize) / roundSize;
        }

        void setZero() {
            x_ = 0.0f;
            y_ = 0.0f;
            z_ = 0.0f;
        };

        const float *data() const {
            return rawVector_;
        }

        //Debug function

        void draw(const Vector &basePoint = Vector(), const VE::Color &color = VE::Color()) const {
            glLineWidth(2);
            glColor3f(color.red(), color.grean(), color.blue());
            glBegin(GL_LINES);
            glVertex3f(basePoint.x(), basePoint.y(), basePoint.z());
            Vector vector = basePoint + *this;
            glVertex3f(vector.x(), vector.y(), vector.z());
            glEnd();
            glPointSize(6);
            glBegin(GL_POINTS);
            glVertex3f(vector.x(), vector.y(), vector.z());
            glEnd();
        }

        void drawPoint(int size = 6, const VE::Color &color = VE::Color()) const {
            glColor3f(color.red(), color.grean(), color.blue());
            glPointSize(size);
            glBegin(GL_POINTS);
            glVertex3f(this->x(), this->y(), this->z());
            glEnd();
        }

        void drawPoint(const VE::Color &color) const {
            drawPoint(6, color);
        }

        void draw(const VE::Color &color) const {
            draw(VE::Vector(), color);
        }

        void print() const {
            std::cout << x_ << " " << y_ << " " << z_ << std::endl;
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
            return (s + (e - s) * t).normalized();
        }

        static Vector slerp(const Vector &s, const Vector &e, float t) {
            if (t < 0.01f) return lerp(s, e, t);

            float theta = angle(s, e);
            float sinTheta = sinf(theta);

            float a = sinf((1.0f - t) * theta) / sinTheta;
            float b = sinf(t * theta) / sinTheta;
            return s * a + e * b;
        }

    private:
        union {
            struct {
                float x_ = 0.0f;
                float y_ = 0.0f;
                float z_ = 0.0f;
            };
            float rawVector_[3];
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
