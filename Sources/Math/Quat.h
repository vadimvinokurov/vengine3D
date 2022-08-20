//
// Created by boris on 12/27/2021.
//

#ifndef VENGINE3D_QUAT_H
#define VENGINE3D_QUAT_H

#include "EngineProperty.h"
#include "Vector.h"
#include "Matrix4.h"

struct Quaternion
{
	__forceinline Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	__forceinline Quaternion(float x_, float y_, float z_, float w_) noexcept : x(x_), y(y_), z(z_), w(w_)
	{
	}

	__forceinline Quaternion(float fillValue) noexcept : x(fillValue), y(fillValue), z(fillValue), w(fillValue)
	{
	}

	__forceinline Quaternion(const Vector3 &v, float w_ = 0.0f) noexcept : x(v.x), y(v.y), z(v.z), w(w_)
	{
	}

	__forceinline Quaternion(const Vector4 &v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w)
	{
	}

	__forceinline Quaternion(const float *fv) noexcept : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3])
	{
	}

	__forceinline Quaternion operator+(const Quaternion &other) const noexcept
	{
		return Quaternion(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
	}

	__forceinline Quaternion operator-(const Quaternion &other) const noexcept
	{
		return Quaternion(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
	}

	__forceinline Quaternion operator*(float f) const noexcept
	{
		return Quaternion(x * f, y * f, z * f, w * f);
	}

	__forceinline Quaternion operator/(float f) const noexcept
	{
		float invf = 1.0f / f;
		return Quaternion(x * invf, y * invf, z * invf, w * invf);
	}

	__forceinline Quaternion operator*(const Quaternion &other) const noexcept
	{
		return Quaternion(this->x * other.w + this->y * other.z - this->z * other.y + this->w * other.x,
						  -this->x * other.z + this->y * other.w + this->z * other.x + this->w * other.y,
						  this->x * other.y - this->y * other.x + this->z * other.w + this->w * other.z,
						  -this->x * other.x - this->y * other.y - this->z * other.z + this->w * other.w);
	}

	__forceinline Quaternion operator/(const Quaternion &b) const noexcept
	{
		return (*this) * b.inverse();
	}

	__forceinline Quaternion &operator+=(const Quaternion &other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	__forceinline Quaternion &operator-=(const Quaternion &other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	__forceinline Quaternion &operator*=(float f) noexcept
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	__forceinline Quaternion &operator/=(float f) noexcept
	{
		float invf = 1.0f / f;
		x *= invf;
		y *= invf;
		z *= invf;
		w *= invf;
		return *this;
	}

	__forceinline Quaternion &operator*=(const Quaternion &other) noexcept
	{
		*this = *this * other;
		return *this;
	}

	__forceinline Quaternion &operator/=(const Quaternion &other) noexcept
	{
		*this = *this / other;
		return *this;
	}

	__forceinline bool operator==(const Quaternion &other) const noexcept
	{
		return (fabsf(this->x - other.x) < FLOAT_EPSILON && fabsf(this->y - other.y) < FLOAT_EPSILON &&
				fabsf(this->z - other.z) < FLOAT_EPSILON && fabsf(this->w - other.w) < FLOAT_EPSILON);
	}

	__forceinline bool operator!=(const Quaternion &other) const noexcept
	{
		return !(*this == other);
	}

	__forceinline bool sameOrientation(const Quaternion &other) const noexcept
	{
		return (fabsf(this->x - other.x) < FLOAT_EPSILON && fabsf(this->y - other.y) < FLOAT_EPSILON &&
				fabsf(this->z - other.z) < FLOAT_EPSILON && fabsf(this->w - other.w) < FLOAT_EPSILON) ||
			   (fabsf(this->x + other.x) < FLOAT_EPSILON && fabsf(this->y + other.y) < FLOAT_EPSILON &&
				fabsf(this->z + other.z) < FLOAT_EPSILON && fabsf(this->w + other.w) < FLOAT_EPSILON);
	}

	__forceinline float dot(const Quaternion &other) const noexcept
	{
		return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
	}

	__forceinline float norma() const noexcept
	{
		return (*this).dot(*this);
	}

	__forceinline float lenSqrt() const
	{
		return x * x + y * y + z * z + w * w;
	}

	__forceinline float len() const noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return 0.0f;
		return sqrtf(norma());
	}

	__forceinline Quaternion getNormalized() const noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return *this;

		return (*this) / sqrtf(lenSq);
	}

	__forceinline Quaternion &normalize() noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return *this;
		(*this) /= sqrtf(lenSq);

		return *this;
	}

	__forceinline Quaternion conjugate() const noexcept
	{
		return Quaternion(x * -1.0f, y * -1.0f, z * -1.0f, w);
	}

	__forceinline Quaternion inverse() const noexcept
	{
		return conjugate() / norma();
	}

	__forceinline Vector3 getAxis() const noexcept
	{
		return Vector3(x, y, z).getNormalized();
	}

	__forceinline float getAngle() const noexcept
	{
		return 2.0 * acosf(w);
	}

	__forceinline Vector3 rotate(const Vector3 &v) const noexcept
	{
		Quaternion tmp = *this * Quaternion(v) * this->inverse();
		return Vector3(tmp.x, tmp.y, tmp.z);
	}

	__forceinline Matrix4 toMatrix4() const noexcept
	{
		Vector3 nx = this->rotate(Vector3(1, 0, 0));
		Vector3 ny = this->rotate(Vector3(0, 1, 0));
		Vector3 nz = this->rotate(Vector3(0, 0, 1));

		return Matrix4(nx.x, nx.y, nx.z, 0.0f, ny.x, ny.y, ny.z, 0.0f, nz.x, nz.y, nz.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	__forceinline static Quaternion fromAxisAngle(const Vector3 &axisAngle) noexcept
	{
		auto [axis, angle] = axisAngle.getNormalAndLen();
		return fromAxisAngle(axis, angle);
	}

	__forceinline static Quaternion fromAxisAngle(const Vector3 &axis, float angle) noexcept
	{
		return Quaternion(axis * sinf(0.5f * angle), cosf(0.5f * angle));
	}

	__forceinline static Quaternion fromTo(const Vector3 &from, const Vector3 &to) noexcept
	{
		Vector3 f = from.getNormalized();
		Vector3 t = to.getNormalized();

		if (f == t)
		{
			return Quaternion();
		}
		else if (f == t * -1.0f)
		{
			Vector3 ortho(1, 0, 0);
			if (fabsf(f.y) < fabsf(f.x))
			{
				ortho = Vector3(0, 1, 0);
			}
			if (fabsf(f.z) < fabsf(f.y) && fabsf(f.z) < fabsf(f.x))
			{
				ortho = Vector3(0, 0, 1);
			}
			return Quaternion((f * ortho).getNormalized(), 0.0f);
		}

		Vector3 half = (f + t).getNormalized();
		Vector3 axis = f * half;
		return Quaternion(axis, f.dot(half));
	}

	__forceinline static Quaternion mix(const Quaternion &from, const Quaternion &to, float t) noexcept
	{
		return from * (1.0f - t) + to * t;
	}

	__forceinline static Quaternion nlerp(const Quaternion &from, const Quaternion &to, float t) noexcept
	{
		return (from + (to - from) * t).getNormalized();
	}

	__forceinline static Quaternion lookRotation(const Vector3 &forward, const Vector3 &up) noexcept
	{
		Vector3 f = forward.getNormalized();
		Vector3 u = up.getNormalized();
		Vector3 r = f * u;
		u = r * f;

		Quaternion worldToObject = fromTo(Vector3(0, 1, 0), f);
		Vector3 objectUp = worldToObject.rotate(Vector3(0, 0, 1));
		Quaternion u2u = fromTo(objectUp, u);

		return (u2u * worldToObject).getNormalized();
	}

	__forceinline static Quaternion fromMatrix(const Matrix4 &m) noexcept
	{
		auto up = toVector3(m.up).getNormalized();
		auto forward = toVector3(m.forward).getNormalized();
		Vector3 right = forward * up;
		up = right * forward;

		return lookRotation(forward, up);
	}

	__forceinline void print() const noexcept
	{
		spdlog::info("{} {} {} {}", x, y, z, w);
	}

	__forceinline const float *data() const noexcept
	{
		return v;
	}

	__forceinline float *data() noexcept
	{
		return v;
	}

	__forceinline static constexpr std::size_t size() noexcept
	{
		return 4;
	}

	union {
		float v[4] = {};
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#endif // VENGINE3D_QUAT_H
