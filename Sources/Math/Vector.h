//
// Created by Boris on 8/24/2021.
//

#ifndef VENGINEMATH_VECTOR_HPP
#define VENGINEMATH_VECTOR_HPP

#include "EngineProperty.h"
#include "EnginePlatform.h"
#include "spdlog/spdlog.h"
struct Vector3
{
	__forceinline Vector3(float fillValue = 0.0f) noexcept : x(fillValue), y(fillValue), z(fillValue){};

	__forceinline Vector3(float x_, float y_, float z_ = 0.0f) noexcept : x(x_), y(y_), z(z_){};

	__forceinline Vector3(const float *fv) noexcept : x(fv[0]), y(fv[1]), z(fv[2]){};

	__forceinline Vector3 operator+(const Vector3 &other) const noexcept
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	__forceinline Vector3 operator-(const Vector3 &other) const noexcept
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	__forceinline Vector3 operator*(float factor) const noexcept
	{
		return Vector3(x * factor, y * factor, z * factor);
	}

	__forceinline Vector3 operator/(float factor) const noexcept
	{
		float invFactor = 1.0f / factor;
		return Vector3(x * invFactor, y * invFactor, z * invFactor);
	}

	__forceinline Vector3 operator*(const Vector3 &other) const noexcept
	{
		const Vector3 &a = (*this);
		const Vector3 &b = other;
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	__forceinline Vector3 &operator+=(const Vector3 &other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	__forceinline Vector3 &operator-=(const Vector3 &other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	__forceinline Vector3 &operator*=(const Vector3 &other) noexcept
	{
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

	__forceinline Vector3 &operator*=(float factor) noexcept
	{
		x *= factor;
		y *= factor;
		z *= factor;

		return *this;
	}

	__forceinline Vector3 &operator/=(float factor) noexcept
	{
		float invFactor = 1.0f / factor;

		x *= invFactor;
		y *= invFactor;
		z *= invFactor;

		return *this;
	}

	__forceinline float dot(const Vector3 &other) const noexcept
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	__forceinline float lenSqrt() const noexcept
	{
		return x * x + y * y + z * z;
	}

	__forceinline float len() const noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return 0.0f;
		return sqrtf(lenSq);
	}

	__forceinline Vector3 getNormalized() const noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return *this;
		return (*this) / sqrtf(lenSq);
	}

	__forceinline Vector3 &normalize() noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
			return *this;
		(*this) /= sqrtf(lenSq);
		return *this;
	}

	__forceinline std::pair<Vector3, float> getNormalAndLen() const noexcept
	{
		float lenSq = lenSqrt();
		if (lenSq < FLOAT_EPSILON)
		{
			return {*this, 0.0f};
		}
		float magnitude = sqrtf(lenSq);
		return {(*this) / magnitude, magnitude};
	}

	__forceinline bool operator==(const Vector3 &other) const noexcept
	{
		return (*this - other).lenSqrt() < FLOAT_EPSILON;
	}

	__forceinline bool operator!=(const Vector3 &other) const noexcept
	{
		return !(*this == other);
	}

	__forceinline Vector3 getProjectionOn(const Vector3 &b) const noexcept
	{
		float bLenSqrt = b.lenSqrt();

		if (bLenSqrt < FLOAT_EPSILON)
			return Vector3();
		float scale = this->dot(b) / bLenSqrt;
		return b * scale;
	}

	__forceinline Vector3 getRejection(const Vector3 &b) const noexcept
	{
		return *this - this->getProjectionOn(b);
	}

	__forceinline Vector3 getReflection(const Vector3 &b) const noexcept
	{
		float bLenSqrt = b.lenSqrt();

		if (bLenSqrt < FLOAT_EPSILON)
			return Vector3();
		float scale = this->dot(b) / bLenSqrt;
		auto projectionX2 = b * scale * 2;
		return *this - projectionX2;
	}

	__forceinline void round(float roundSize = 0.0001f) noexcept
	{
		float invRoundSize = 1.0f / roundSize;

		x = static_cast<int>(x * invRoundSize) * roundSize;
		y = static_cast<int>(y * invRoundSize) * roundSize;
		z = static_cast<int>(z * invRoundSize) * roundSize;
	}

	__forceinline void setZero() noexcept
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};

	__forceinline const float *data() const noexcept
	{
		return v;
	}

	__forceinline float *data() noexcept
	{
		return v;
	}

	__forceinline void print() const
	{
		spdlog::info("{} {} {}", x, y, z);
	}

	__forceinline static float angle(const Vector3 &a, const Vector3 &b) noexcept
	{
		float aLenSqrt = a.lenSqrt();
		float bLenSqrt = b.lenSqrt();
		if (aLenSqrt < FLOAT_EPSILON || bLenSqrt < FLOAT_EPSILON)
		{
			return 0.0f;
		}
		return acosf(a.dot(b) / (sqrtf(aLenSqrt) * sqrtf(bLenSqrt)));
	}

	__forceinline static Vector3 lerp(const Vector3 &s, const Vector3 &e, float t) noexcept
	{
		// return (1 - t) * s + t * e;
		return s + (e - s) * t;
	}

	__forceinline static Vector3 nlerp(const Vector3 &s, const Vector3 &e, float t) noexcept
	{
		return lerp(s, e, t).getNormalized();
	}

	__forceinline static Vector3 slerp(const Vector3 &s, const Vector3 &e, float t) noexcept
	{
		if (t < 0.01f)
			return lerp(s, e, t);

		float theta = angle(s, e);
		float sinTheta = sinf(theta);

		float a = sinf((1.0f - t) * theta) / sinTheta;
		float b = sinf(t * theta) / sinTheta;
		return s * a + e * b;
	}

	__forceinline static constexpr std::size_t size() noexcept
	{
		return 3;
	}

	union {
		float v[3] = {};
		struct
		{
			float x;
			float y;
			float z;
		};
	};
};

__forceinline Vector3 operator*(float factor, const Vector3 &v) noexcept
{
	return v * factor;
}

__forceinline Vector3 multiplyComponents(const Vector3 &a, const Vector3 &b) noexcept
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

__forceinline bool sameDirection(const Vector3 &a, const Vector3 &b) noexcept
{
	return a.dot(b) > 0.0f;
};

template <typename T>
struct TVector2
{
	TVector2() : x(static_cast<T>(0)), y(static_cast<T>(0))
	{
	}

	TVector2(T x_, T y_) : x(x_), y(y_)
	{
	}

	TVector2(const T *fv) : x(fv[0]), y(fv[1])
	{
	}

	const T *data() const
	{
		return v;
	}

	T *data()
	{
		return v;
	}

	static constexpr std::size_t size()
	{
		return 2;
	}

	union {
		T v[2] = {};
		struct
		{
			T x;
			T y;
		};
	};
};

using Vector2 = TVector2<float>;
using IVector2 = TVector2<int32>;

template <typename T>
struct TVector4
{
	TVector4() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0))
	{
	}

	TVector4(T x_, T y_, T z_, T w_ = 1.0f) : x(x_), y(y_), z(z_), w(w_)
	{
	}

	TVector4(const T *fv) : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3])
	{
	}

	TVector4 &operator+=(float other)
	{
		x += other;
		y += other;
		z += other;
		w += other;
		return *this;
	}

	const T *data() const
	{
		return v;
	}

	T *data()
	{
		return v;
	}

	static constexpr std::size_t size()
	{
		return 4;
	}

	union {
		T v[4] = {};
		struct
		{
			T x;
			T y;
			T z;
			T w;
		};
		struct
		{
			T red;
			T green;
			T blue;
			T alfa;
		};
	};
};

using Vector4 = TVector4<float>;
using Color = TVector4<float>;
using IVector4 = TVector4<int32>;
using UIVector4 = TVector4<uint32>;

inline auto toVector3(const Vector4 &v4)
{
	return Vector3(v4.x, v4.y, v4.z);
}

#endif // VENGINEMATH_VECTOR_HPP
