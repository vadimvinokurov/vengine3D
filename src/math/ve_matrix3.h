//
// Created by boris on 2/6/2022.
//

#ifndef VENGINE3D_VE_MATRIX3_H
#define VENGINE3D_VE_MATRIX3_H

//#include "ve_matrix33.h"
#include "ve_vector.h"

namespace VE {
    struct Matrix3 {
        static constexpr float EPSILON = 0.000001f;

        Matrix3() : _00(1), _01(0), _02(0),
                    _10(0), _11(1), _12(0),
                    _20(0), _21(0), _22(1) {}

        Matrix3(float _v00, float _v01, float _v02,
                float _v10, float _v11, float _v12,
                float _v20, float _v21, float _v22)
                : _00(_v00), _01(_v01), _02(_v02),
                  _10(_v10), _11(_v11), _12(_v12),
                  _20(_v20), _21(_v21), _22(_v22) {}

        Matrix3(float *fv) :
                _00(fv[0]), _01(fv[1]), _02(fv[2]),
                _10(fv[3]), _11(fv[4]), _12(fv[5]),
                _20(fv[6]), _21(fv[7]), _22(fv[8]) {}

        bool operator==(const Matrix3 &b) const {
            for (size_t i = 0; i < 9; ++i) {
                if (fabsf(this->v[i] - b.v[i]) > EPSILON) return false;
            }
            return true;
        }

        bool operator!=(const Matrix3 &b) const {
            return !(*this == b);
        }

        Matrix3 operator+(const Matrix3 &b) const {
            const Matrix3 &a = *this;
            return Matrix3(
                    a.v[0] + b.v[0],
                    a.v[1] + b.v[1],
                    a.v[2] + b.v[2],
                    a.v[3] + b.v[3],
                    a.v[4] + b.v[4],
                    a.v[5] + b.v[5],
                    a.v[6] + b.v[6],
                    a.v[7] + b.v[7],
                    a.v[8] + b.v[8]
            );
        }

        Matrix3 operator-(const Matrix3 &b) const {
            const Matrix3 &a = *this;
            return Matrix3(
                    a.v[0] - b.v[0],
                    a.v[1] - b.v[1],
                    a.v[2] - b.v[2],
                    a.v[3] - b.v[3],
                    a.v[4] - b.v[4],
                    a.v[5] - b.v[5],
                    a.v[6] - b.v[6],
                    a.v[7] - b.v[7],
                    a.v[8] - b.v[8]
            );
        }

        Matrix3 operator*(float f) const {
            return Matrix3(
                    this->v[0] * f,
                    this->v[1] * f,
                    this->v[2] * f,
                    this->v[3] * f,
                    this->v[4] * f,
                    this->v[5] * f,
                    this->v[6] * f,
                    this->v[7] * f,
                    this->v[8] * f);
        }

        Matrix3 operator/(float f) const {
            f = 1.0f / f;
            return Matrix3(
                    this->v[0] * f,
                    this->v[1] * f,
                    this->v[2] * f,
                    this->v[3] * f,
                    this->v[4] * f,
                    this->v[5] * f,
                    this->v[6] * f,
                    this->v[7] * f,
                    this->v[8] * f
            );
        }

        Matrix3 &operator+=(const Matrix3 &b) {
            for (size_t i = 0; i < 9; ++i) {
                this->v[i] += b.v[i];
            }
            return *this;
        }

        Matrix3 &operator-=(const Matrix3 &b) {
            for (size_t i = 0; i < 9; ++i) {
                this->v[i] -= b.v[i];
            }
            return *this;
        }

        Matrix3 &operator*=(float f) {
            for (size_t i = 0; i < 9; ++i) {
                this->v[i] *= f;
            }
            return *this;
        }

        Matrix3 &operator/=(float f) {
            f = 1.0f / f;
            for (size_t i = 0; i < 16; ++i) {
                this->v[i] *= f;
            }
            return *this;
        }

        Matrix3 operator*(const Matrix3 &b) const {
            return Matrix3(
                    this->v[0] * b.v[0] + this->v[1] * b.v[3] + this->v[2] * b.v[6],
                    this->v[0] * b.v[1] + this->v[1] * b.v[4] + this->v[2] * b.v[7],
                    this->v[0] * b.v[2] + this->v[1] * b.v[5] + this->v[2] * b.v[8],

                    this->v[3] * b.v[0] + this->v[4] * b.v[3] + this->v[5] * b.v[6],
                    this->v[3] * b.v[1] + this->v[4] * b.v[4] + this->v[5] * b.v[7],
                    this->v[3] * b.v[2] + this->v[4] * b.v[5] + this->v[5] * b.v[8],

                    this->v[6] * b.v[0] + this->v[7] * b.v[3] + this->v[8] * b.v[6],
                    this->v[6] * b.v[1] + this->v[7] * b.v[4] + this->v[8] * b.v[7],
                    this->v[6] * b.v[2] + this->v[7] * b.v[5] + this->v[8] * b.v[8]);
        }

        Vector operator*(const Vector &vector) const {
            return Vector(
                    this->v[0] * vector.v[0] + this->v[1] * vector.v[1] + this->v[2] * vector.v[2],
                    this->v[3] * vector.v[0] + this->v[4] * vector.v[1] + this->v[5] * vector.v[2],
                    this->v[6] * vector.v[0] + this->v[7] * vector.v[1] + this->v[8] * vector.v[2]
            );
        }


        Matrix3 getTransposed() const {
            return Matrix3(_00, _10, _20,
                           _01, _11, _21,
                           _02, _12, _22);
        }

        Matrix3 &transpose() {
            std::swap(_01, _10);
            std::swap(_02, _20);
            std::swap(_12, _21);
            return *this;
        }

        float determinant() const {
            float p = v[0] * v[4] * v[8] + v[1] * v[5] * v[6] + v[2] * v[3] * v[7];
            float m = v[2] * v[4] * v[6] + v[0] * v[5] * v[7] + v[1] * v[3] * v[8];
            return p - m;
        }

        Matrix3 getInversed() const {
            Matrix3 res;
            float det = determinant();
            if (det == 0.0f) return Matrix3();

            Matrix3 cofactor;

            cofactor.v[0] = v[4] * v[8] - v[7] * v[5];
            cofactor.v[3] = v[6] * v[5] - v[3] * v[8];
            cofactor.v[6] = v[3] * v[7] - v[6] * v[4];

            cofactor.v[1] = v[7] * v[2] - v[1] * v[8];
            cofactor.v[4] = v[0] * v[8] - v[6] * v[2];
            cofactor.v[7] = v[6] * v[1] - v[0] * v[7];

            cofactor.v[2] = v[1] * v[5] - v[4] * v[2];
            cofactor.v[5] = v[3] * v[2] - v[0] * v[5];
            cofactor.v[8] = v[0] * v[4] - v[3] * v[1];

            return cofactor * (1.0f / det);
        }

        Matrix3 &inverse() {
            *this = this->getInversed();
            return *this;
        }

        Matrix3 &setZero() {
            memset(v, 0, sizeof(float) * 9);
            return *this;
        }

        Matrix3 &setIdentity() {
            v[0] = 1;
            v[1] = 0;
            v[2] = 0;
            v[3] = 0;
            v[4] = 1;
            v[5] = 0;
            v[6] = 0;
            v[7] = 0;
            v[8] = 1;
            return *this;
        }

        float* data(){
            return static_cast<float*>(v);
        }

        void print() const {
            std::cout << std::endl;
            std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << std::endl;
            std::cout << v[3] << "\t" << v[4] << "\t" << v[5] << std::endl;
            std::cout << v[6] << "\t" << v[7] << "\t" << v[8] << std::endl;
            std::cout << std::endl;
        }

        union {
            float v[9] = {0};
            struct {
                float _00;
                float _01;
                float _02;
                float _10;
                float _11;
                float _12;
                float _20;
                float _21;
                float _22;
            };
        };
    };

    inline Vector operator*(const Vector &vector, const Matrix3& m) {
        return Vector(
                m.v[0] * vector.v[0] + m.v[3] * vector.v[1] + m.v[6] * vector.v[2],
                m.v[1] * vector.v[0] + m.v[4] * vector.v[1] + m.v[7] * vector.v[2],
                m.v[2] * vector.v[0] + m.v[5] * vector.v[1] + m.v[8] * vector.v[2]
        );
    }
}


#endif //VENGINE3D_VE_MATRIX3_H
