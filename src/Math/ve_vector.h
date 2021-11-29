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
    class Vector {
    public:
        Vector() {};

        explicit Vector(float x, float y, float z = 0) : x_(x), y_(y), z_(z) {};

        Vector(const Vector &vector) = default;

        Vector &operator=(const Vector &vector) = default;

        bool operator==(const Vector &other) const {
            return ((x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_));
        }

        Vector operator+(const Vector &other) const {
            return Vector((*this).x_ + other.x_, (*this).y_ + other.y_, (*this).z_ + other.z_);
        }

        Vector operator-(const Vector &other) const {
            return Vector((*this).x_ - other.x_, (*this).y_ - other.y_, (*this).z_ - other.z_);
        }

        Vector operator*(float factor) const {
            return Vector(x_ * factor, y_ * factor, z_ * factor);
        }

        Vector operator/(float factor) const {
            return Vector(x_ / factor, y_ / factor, z_ / factor);
        }

        Vector operator*(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return Vector(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
        }

        Vector &operator+=(const Vector &other) {
            this->x_ += other.x_;
            this->y_ += other.y_;
            this->z_ += other.z_;
            return *this;
        }

        Vector &operator-=(const Vector &other) {
            this->x_ -= other.x_;
            this->y_ -= other.y_;
            this->z_ -= other.z_;
            return *this;
        }

        Vector &operator*=(const Vector &other) {
            const Vector &a = (*this);
            const Vector &b = other;

            this->x_ = a.y() * b.z() - a.z() * b.y();
            this->y_ = a.z() * b.x() - a.x() * b.z();
            this->z_ = a.x() * b.y() - a.y() * b.x();

            return *this;
        }

        Vector &operator*=(float factor) {
            this->x_ *= factor;
            this->y_ *= factor;
            this->z_ *= factor;

            return *this;
        }

        Vector &operator/=(float factor) {
            this->x_ /= factor;
            this->y_ /= factor;
            this->z_ /= factor;

            return *this;
        }

        bool operator!=(const Vector &other) const {
            return !(*this == other);
        }

        float sqrtAbs() const {
            return x_ * x_ + y_ * y_ + z_ * z_;
        }

        float abs() const {
            return sqrt(sqrtAbs());
        }

        Vector normolize() const {
            if (abs() == 0) {
                return Vector(0, 0, 0);
            }
            return Vector(x_ / abs(), y_ / abs(), z_ / abs());
        }

        Vector mirror(const VE::Vector &normal) const {
            float scalarRes = dot(normal);
            if (scalarRes > 0)
                return (*this);
            return (*this) + normal * scalarRes * (-2);
        }

        float getAngle(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return acosf(a.dot(b) / (a.abs() * b.abs()));
        }

        float dot(const Vector &other) const {
            const Vector &a = (*this);
            const Vector &b = other;
            return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
        }

        float x() const { return x_; }

        float y() const { return y_; }

        float z() const { return z_; }

        void setX(float x) { x_ = x; }

        void setY(float y) { y_ = y; }

        void setZ(float z) { z_ = z; }

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
            return c;
        }

    private:
        union {
            struct {
                float x_ = 0.0f;
                float y_ = 0.0f;
                float z_ = 0.0f;
            };
            float c[3];
        };

    };

    inline bool sameDirection(const Vector &a, const Vector &b) {
        return a.dot(b) > 0;
    }
}


#endif //VOLLEYBALL_VECTOR_HPP
