//
// Created by boris on 2/5/2022.
//

#ifndef VENGINE3D_VE_MATRIX4_H
#define VENGINE3D_VE_MATRIX4_H

#include <cmath>
#include "ve_vector4.h"

#define M4D(aRow, bCol) \
    a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] + \
    a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] + \
    a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] + \
    a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3]

#define M4V4D(aRow, x, y, z, w) \
    a.v[0 * 4 + aRow] * x + \
    a.v[1 * 4 + aRow] * y + \
    a.v[2 * 4 + aRow] * z + \
    a.v[3 * 4 + aRow] * w


namespace VE {
    struct Matrix4 {
        static constexpr float EPSILON = 0.000001f;

        Matrix4() {}

        Matrix4(float _00, float _01, float _02, float _03,
                float _10, float _11, float _12, float _13,
                float _20, float _21, float _22, float _23,
                float _30, float _31, float _32, float _33) :
                xx(_00), xy(_01), xz(_02), xw(_03),
                yx(_10), yy(_11), yz(_12), yw(_13),
                zx(_20), zy(_21), zz(_22), zw(_23),
                tx(_30), ty(_31), tz(_32), tw(_33) {}

        Matrix4(float *fv) :
                xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
                yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
                zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
                tx(fv[12]), ty(fv[13]), tz(fv[14]), tw(fv[15]) {}

        bool operator==(const Matrix4 &b) {
            for (size_t i = 0; i < 16; ++i) {
                if (fabsf(this->v[i] - b.v[i]) > EPSILON) return false;
            }
            return true;
        }

        bool operator!=(const Matrix4 &b) {
            return !(*this == b);
        }

        Matrix4 operator+(const Matrix4 &b) {
            const Matrix4 &a = *this;
            return Matrix4(
                    a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
                    a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
                    a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
                    a.tx + b.tx, a.ty + b.ty, a.tz + b.tz, a.tw + b.tw
            );
        }

        Matrix4 operator-(const Matrix4 &b) {
            const Matrix4 &a = *this;
            return Matrix4(
                    a.xx - b.xx, a.xy - b.xy, a.xz - b.xz, a.xw - b.xw,
                    a.yx - b.yx, a.yy - b.yy, a.yz - b.yz, a.yw - b.yw,
                    a.zx - b.zx, a.zy - b.zy, a.zz - b.zz, a.zw - b.zw,
                    a.tx - b.tx, a.ty - b.ty, a.tz - b.tz, a.tw - b.tw
            );
        }

        Matrix4 operator*(float f) {
            return Matrix4(
                    xx * f, xy * f, xz * f, xw * f,
                    yx * f, yy * f, yz * f, yw * f,
                    zx * f, zy * f, zz * f, zw * f,
                    tx * f, ty * f, tz * f, tw * f
            );
        }

        Matrix4 operator/(float f) {
            f = 1.0f / f;
            return Matrix4(
                    xx * f, xy * f, xz * f, xw * f,
                    yx * f, yy * f, yz * f, yw * f,
                    zx * f, zy * f, zz * f, zw * f,
                    tx * f, ty * f, tz * f, tw * f
            );
        }

        Matrix4 &operator+=(const Matrix4 &b) {
            for (size_t i = 0; i < 16; ++i) {
                this->v[i] += b.v[i];
            }
            return *this;
        }

        Matrix4 &operator-=(const Matrix4 &b) {
            for (size_t i = 0; i < 16; ++i) {
                this->v[i] -= b.v[i];
            }
            return *this;
        }

        Matrix4 &operator*=(float f) {
            for (size_t i = 0; i < 16; ++i) {
                this->v[i] *= f;
            }
            return *this;
        }

        Matrix4 &operator/=(float f) {
            f = 1.0f / f;
            for (size_t i = 0; i < 16; ++i) {
                this->v[i] *= f;
            }
            return *this;
        }

        Matrix4 operator*(const Matrix4 &b) {
            const Matrix4 &a = *this;
            return Matrix4(
                    M4D(0, 0), M4D(1, 0), M4D(2, 0), M4D(3, 0),
                    M4D(0, 1), M4D(1, 1), M4D(2, 1), M4D(3, 1),
                    M4D(0, 2), M4D(1, 2), M4D(2, 2), M4D(3, 2),
                    M4D(0, 3), M4D(1, 3), M4D(2, 3), M4D(3, 3)
            );
        }

        Vector4 operator*(const Vector4 &v4) {
            const Matrix4 &a = *this;
            return Vector4{M4V4D(0, v4.x, v4.y, v4.z, v4.w),
                           M4V4D(1, v4.x, v4.y, v4.z, v4.w),
                           M4V4D(2, v4.x, v4.y, v4.z, v4.w),
                           M4V4D(3, v4.x, v4.y, v4.z, v4.w)};

        }


        union {
            float v[16] = {0};
            struct {
                Vector4 right;
                Vector4 up;
                Vector4 forward;
                Vector4 position;
            };
            struct {
                float xx;
                float xy;
                float xz;
                float xw;
                float yx;
                float yy;
                float yz;
                float yw;
                float zx;
                float zy;
                float zz;
                float zw;
                float tx;
                float ty;
                float tz;
                float tw;
            };
            struct {//column-row notation
                float c0r0;
                float c0r1;
                float c0r2;
                float c0r3;
                float c1r0;
                float c1r1;
                float c1r2;
                float c1r3;
                float c2r0;
                float c2r1;
                float c2r2;
                float c2r3;
                float c3r0;
                float c3r1;
                float c3r2;
                float c3r3;
            };
            struct {//row-column notation
                float r0c0;
                float r1c0;
                float r2c0;
                float r3c0;
                float r0c1;
                float r1c1;
                float r2c1;
                float r3c1;
                float r0c2;
                float r1c2;
                float r2c2;
                float r3c2;
                float r0c3;
                float r1c3;
                float r2c3;
                float r3c3;
            };
        };
    };
}


#endif //VENGINE3D_VE_MATRIX4_H
