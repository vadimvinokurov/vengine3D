//
// Created by boris on 2/5/2022.
//

#ifndef VENGINE3D_VE_MATRIX4_H
#define VENGINE3D_VE_MATRIX4_H

#include "EngineProperty.h"
#include <cmath>
#include "Vector.h"

// clang-format off
#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
    (x[c0 * 4 + r0] * (x[c1 * 4 + r1] * x[c2 * 4 + r2] - x[c1 * 4 + r2] * x[c2 * 4 + r1]) - \
     x[c1 * 4 + r0] * (x[c0 * 4 + r1] * x[c2 * 4 + r2] - x[c0 * 4 + r2] * x[c2 * 4 + r1]) + \
     x[c2 * 4 + r0] * (x[c0 * 4 + r1] * x[c1 * 4 + r2] - x[c0 * 4 + r2] * x[c1 * 4 + r1]))

struct Matrix4 {
	__forceinline Matrix4() noexcept :
					_00(1), _01(0), _02(0), _03(0),
					_10(0), _11(1), _12(0), _13(0),
					_20(0), _21(0), _22(1), _23(0),
					_30(0), _31(0), _32(0), _33(1) {}

	__forceinline Matrix4(float _v00, float _v01, float _v02, float _v03,
						  float _v10, float _v11, float _v12, float _v13,
						  float _v20, float _v21, float _v22, float _v23,
						  float _v30, float _v31, float _v32, float _v33) noexcept :
							_00(_v00), _01(_v01), _02(_v02), _03(_v03),
							_10(_v10), _11(_v11), _12(_v12), _13(_v13),
							_20(_v20), _21(_v21), _22(_v22), _23(_v23),
							_30(_v30), _31(_v31), _32(_v32), _33(_v33) {}

	__forceinline Matrix4(const float *fv) noexcept :
				_00(fv[0 ]), _01(fv[4 ]), _02(fv[8 ]), _03(fv[12 ]),
				_10(fv[1 ]), _11(fv[5 ]), _12(fv[9 ]), _13(fv[13 ]),
				_20(fv[2 ]), _21(fv[6 ]), _22(fv[10]), _23(fv[14]),
				_30(fv[3 ]), _31(fv[7 ]), _32(fv[11]), _33(fv[15]) {}

	__forceinline bool operator==(const Matrix4 &other) const noexcept {
		for (size_t i = 0; i < 16; ++i) {
			if (fabsf(this->v[i] - other.v[i]) > FLOAT_EPSILON) return false;
		}
		return true;
	}

	__forceinline bool operator!=(const Matrix4 &other) const noexcept {
		return !(*this == other);
	}

	__forceinline Matrix4 operator+(const Matrix4 &other) const noexcept {
		float v[16];
		for(int i = 0; i < 16; ++i) {
			v[i] = this->v[i] + other.v[i];
		}
		return Matrix4(v);
	}

	__forceinline Matrix4 operator-(const Matrix4 &other) const noexcept {
		float v[16];
		for(int i = 0; i < 16; ++i) {
			v[i] = this->v[i] - other.v[i];
		}
		return Matrix4(v);
	}

	__forceinline Matrix4 operator*(float f) const noexcept {
		float v[16];
		for(int i = 0; i < 16; ++i) {
			v[i] = this->v[i] * f;
		}
		return Matrix4(v);
	}

	__forceinline Matrix4 operator/(float f) const noexcept {
		f = 1.0f / f;
		float v[16];
		for(int i = 0; i < 16; ++i) {
			v[i] = this->v[i] * f;
		}
		return Matrix4(v);
	}

	__forceinline Matrix4 &operator+=(const Matrix4 &other) noexcept {
		for (size_t i = 0; i < 16; ++i) {
			this->v[i] += other.v[i];
		}
		return *this;
	}

	__forceinline Matrix4 &operator-=(const Matrix4 &other) noexcept {
		for (size_t i = 0; i < 16; ++i) {
			this->v[i] -= other.v[i];
		}
		return *this;
	}

	__forceinline Matrix4 &operator*=(float f) noexcept {
		for (size_t i = 0; i < 16; ++i) {
			this->v[i] *= f;
		}
		return *this;
	}

	__forceinline Matrix4 &operator/=(float f) noexcept {
		f = 1.0f / f;
		for (size_t i = 0; i < 16; ++i) {
			this->v[i] *= f;
		}
		return *this;
	}

	__forceinline Matrix4 operator*(const Matrix4 &other) const noexcept {
		return Matrix4(
				this->v[0] * other.v[0] + this->v[4] * other.v[1] + this->v[8] * other.v[2] + this->v[12] * other.v[3], //0
				this->v[1] * other.v[0] + this->v[5] * other.v[1] + this->v[9] * other.v[2] + this->v[13] * other.v[3], // 1
				this->v[2] * other.v[0] + this->v[6] * other.v[1] + this->v[10] * other.v[2] + this->v[14] * other.v[3], // 2
				this->v[3] * other.v[0] + this->v[7] * other.v[1] + this->v[11] * other.v[2] + this->v[15] * other.v[3], //3
				this->v[0] * other.v[4] + this->v[4] * other.v[5] + this->v[8] * other.v[6] + this->v[12] * other.v[7], // 4
				this->v[1] * other.v[4] + this->v[5] * other.v[5] + this->v[9] * other.v[6] + this->v[13] * other.v[7], // 5
				this->v[2] * other.v[4] + this->v[6] * other.v[5] + this->v[10] * other.v[6] + this->v[14] * other.v[7], // 6
				this->v[3] * other.v[4] + this->v[7] * other.v[5] + this->v[11] * other.v[6] + this->v[15] * other.v[7], // 7
				this->v[0] * other.v[8] + this->v[4] * other.v[9] + this->v[8] * other.v[10] + this->v[12] * other.v[11], // 8
				this->v[1] * other.v[8] + this->v[5] * other.v[9] + this->v[9] * other.v[10] + this->v[13] * other.v[11], // 9
				this->v[2] * other.v[8] + this->v[6] * other.v[9] + this->v[10] * other.v[10] + this->v[14] * other.v[11], // 10
				this->v[3] * other.v[8] + this->v[7] * other.v[9] + this->v[11] * other.v[10] + this->v[15] * other.v[11], // 11
				this->v[0] * other.v[12] + this->v[4] * other.v[13] + this->v[8] * other.v[14] + this->v[12] * other.v[15], // 12
				this->v[1] * other.v[12] + this->v[5] * other.v[13] + this->v[9] * other.v[14] + this->v[13] * other.v[15], // 13
				this->v[2] * other.v[12] + this->v[6] * other.v[13] + this->v[10] * other.v[14] + this->v[14] * other.v[15], // 14
				this->v[3] * other.v[12] + this->v[7] * other.v[13] + this->v[11] * other.v[14] + this->v[15] * other.v[15]); // 15
	}

	__forceinline Vector4 operator*(const Vector4 &other) const noexcept {
		return Vector4{this->v[0] * other.v[0] + this->v[4] * other.v[1] + this->v[8] * other.v[2] + this->v[12] * other.v[3],
					   this->v[1] * other.v[0] + this->v[5] * other.v[1] + this->v[9] * other.v[2] + this->v[13] * other.v[3],
					   this->v[2] * other.v[0] + this->v[6] * other.v[1] + this->v[10] * other.v[2] + this->v[14] * other.v[3],
					   this->v[3] * other.v[0] + this->v[7] * other.v[1] + this->v[11] * other.v[2] + this->v[15] * other.v[3]};
	}

	__forceinline Vector3 transformPoint(const Vector3 &vector) const noexcept {
		return Vector3(this->v[0] * vector.v[0] + this->v[4] * vector.v[1] + this->v[8] * vector.v[2] + this->v[12] * 1.0f,
					   this->v[1] * vector.v[0] + this->v[5] * vector.v[1] + this->v[9] * vector.v[2] + this->v[13] * 1.0f,
					   this->v[2] * vector.v[0] + this->v[6] * vector.v[1] + this->v[10] * vector.v[2] + this->v[14] * 1.0f);
	}

	__forceinline Vector3 transformVector(const Vector3 &vector) const noexcept {
	return Vector3(this->v[0] * vector.v[0] + this->v[4] * vector.v[1] + this->v[8] * vector.v[2],
				   this->v[1] * vector.v[0] + this->v[5] * vector.v[1] + this->v[9] * vector.v[2],
				   this->v[2] * vector.v[0] + this->v[6] * vector.v[1] + this->v[10] * vector.v[2]);
  }

	__forceinline Vector3 transformVector(const Vector3 &vector, float &w) const noexcept {
		float tmpw = w;
		w = this->v[3] * vector.v[0] + this->v[7] * vector.v[1] + this->v[11] * vector.v[2] + this->v[15] * tmpw;

		return Vector3(this->v[0] * vector.v[0] + this->v[4] * vector.v[1] + this->v[8] * vector.v[2] + this->v[12] * tmpw,
					   this->v[1] * vector.v[0] + this->v[5] * vector.v[1] + this->v[9] * vector.v[2] + this->v[13] * tmpw,
					   this->v[2] * vector.v[0] + this->v[6] * vector.v[1] + this->v[10] * vector.v[2] + this->v[14] * tmpw);
	}

	__forceinline Matrix4 getTransposed() const noexcept {
		return Matrix4(_00, _10, _20, _30,
					   _01, _11, _21, _31,
					   _02, _12, _22, _32,
					   _03, _13, _23, _33);
	}

	__forceinline Matrix4 &transpose() noexcept {
		std::swap(_10, _01);
		std::swap(_20, _02);
		std::swap(_30, _03);
		std::swap(_21, _12);
		std::swap(_31, _13);
		std::swap(_32, _23);
		return *this;
	}

	__forceinline float determinant() const noexcept {
		return this->v[0] * M4_3X3MINOR(this->v, 1, 2, 3, 1, 2, 3)
			   - this->v[4] * M4_3X3MINOR(this->v, 0, 2, 3, 1, 2, 3)
			   + this->v[8] * M4_3X3MINOR(this->v, 0, 1, 3, 1, 2, 3)
			   - this->v[12] * M4_3X3MINOR(this->v, 0, 1, 2, 1, 2, 3);
	}

	__forceinline Matrix4 getInversed() const noexcept {
		float det = this->determinant();
		if (det == 0.0f) {
			return Matrix4();
		}

		//Cof (M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
		Matrix4 cofactor;
		cofactor.v[0] = M4_3X3MINOR(this->v, 1, 2, 3, 1, 2, 3);
		cofactor.v[4] = -M4_3X3MINOR(this->v, 1, 2, 3, 0, 2, 3);
		cofactor.v[8] = M4_3X3MINOR(this->v, 1, 2, 3, 0, 1, 3);
		cofactor.v[12] = -M4_3X3MINOR(this->v, 1, 2, 3, 0, 1, 2);

		cofactor.v[1] = -M4_3X3MINOR(this->v, 0, 2, 3, 1, 2, 3);
		cofactor.v[5] = M4_3X3MINOR(this->v, 0, 2, 3, 0, 2, 3);
		cofactor.v[9] = -M4_3X3MINOR(this->v, 0, 2, 3, 0, 1, 3);
		cofactor.v[13] = M4_3X3MINOR(this->v, 0, 2, 3, 0, 1, 2);

		cofactor.v[2] = M4_3X3MINOR(this->v, 0, 1, 3, 1, 2, 3);
		cofactor.v[6] = -M4_3X3MINOR(this->v, 0, 1, 3, 0, 2, 3);
		cofactor.v[10] = M4_3X3MINOR(this->v, 0, 1, 3, 0, 1, 3);
		cofactor.v[14] = -M4_3X3MINOR(this->v, 0, 1, 3, 0, 1, 2);

		cofactor.v[3] = -M4_3X3MINOR(this->v, 0, 1, 2, 1, 2, 3);
		cofactor.v[7] = M4_3X3MINOR(this->v, 0, 1, 2, 0, 2, 3);
		cofactor.v[11] = -M4_3X3MINOR(this->v, 0, 1, 2, 0, 1, 3);
		cofactor.v[15] = M4_3X3MINOR(this->v, 0, 1, 2, 0, 1, 2);

		return cofactor * (1.0f / det);
	}

	__forceinline Matrix4 &inverse() noexcept {
		*this = this->getInversed();
		return *this;
	}

	__forceinline Matrix4 &setZero() noexcept {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		v[3] = 0;
		v[4] = 0;
		v[5] = 0;
		v[6] = 0;
		v[7] = 0;
		v[8] = 0;
		v[9] = 0;
		v[10] = 0;
		v[11] = 0;
		v[12] = 0;
		v[13] = 0;
		v[14] = 0;
		v[15] = 0;
		return *this;
	}

	__forceinline Matrix4 &setIdentity() noexcept {
		v[0] = 1;
		v[1] = 0;
		v[2] = 0;
		v[3] = 0;
		v[4] = 0;
		v[5] = 1;
		v[6] = 0;
		v[7] = 0;
		v[8] = 0;
		v[9] = 0;
		v[10] = 1;
		v[11] = 0;
		v[12] = 0;
		v[13] = 0;
		v[14] = 0;
		v[15] = 1;
		return *this;
	}

	__forceinline const float *data() const noexcept {
		return v;
	}

	__forceinline float *data() {
		return v;
	}

	__forceinline static constexpr std::size_t size() noexcept{
		return 16;
	}

	__forceinline void print() const noexcept {
		spdlog::info("\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n\n",
														_00, _01, _02, _03,
														_10, _11, _12, _13,
														_20, _21, _22, _23,
														_30, _31, _32, _33);
	}

	union {
		float v[16] = {};
		struct {
			Vector4 right;
			Vector4 forward;
			Vector4 up;
			Vector4 position;
		};
		struct {
			float _00;
			float _10;
			float _20;
			float _30;
			float _01;
			float _11;
			float _21;
			float _31;
			float _02;
			float _12;
			float _22;
			float _32;
			float _03;
			float _13;
			float _23;
			float _33;
		};
	};
};

inline Vector4 operator*(const Vector4 &b, const Matrix4 &a) {
	return Vector4{a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2] + a.v[3] * b.v[3],
				   a.v[4] * b.v[0] + a.v[5] * b.v[1] + a.v[6] * b.v[2] + a.v[7] * b.v[3],
				   a.v[8] * b.v[0] + a.v[9] * b.v[1] + a.v[10] * b.v[2] + a.v[11] * b.v[3],
				   a.v[12] * b.v[0] + a.v[13] * b.v[1] + a.v[14] * b.v[2] + a.v[15] * b.v[3]};
}

// clang-format on

#endif // VENGINE3D_VE_MATRIX4_H
