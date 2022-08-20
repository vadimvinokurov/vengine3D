//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "EngineProperty.h"
#include "Math/Vector.h"
#include "Math/Matrix3.h"
#include "Math/Quat.h"

struct Transform
{
	static constexpr float EPSILON = 0.000001f;

	__forceinline Transform() noexcept : position(0, 0, 0), rotation(0, 0, 0, 1), scale(1, 1, 1)
	{
	}

	__forceinline Transform(const Vector3 &_position, const Quaternion &_rotation, const Vector3 &_scale) noexcept
		: position(_position), rotation(_rotation), scale(_scale)
	{
	}

	__forceinline Transform(const Vector3 &_position) noexcept
		: position(_position), rotation(0, 0, 0, 1), scale(1, 1, 1)
	{
	}

	__forceinline Transform(const Quaternion &_rotation) noexcept
		: position(0, 0, 0), rotation(_rotation), scale(1, 1, 1)
	{
	}

	__forceinline Transform operator*(const Transform &other) const noexcept
	{
		Transform out;
		out.scale = multiplyComponents(this->scale, other.scale);
		out.rotation = this->rotation * other.rotation;

		out.position = this->rotation.rotate(multiplyComponents(this->scale, other.position));
		out.position += this->position;

		return out;
	}

	__forceinline Vector3 applyToPoint(const Vector3 &localPoint) const noexcept
	{
		return rotation.rotate(multiplyComponents(localPoint, scale)) + position;
	}

	__forceinline Vector3 applyToVector(const Vector3 &localPoint) const noexcept
	{
		return rotation.rotate(multiplyComponents(localPoint, scale));
	}

	__forceinline Transform getInversed() const noexcept
	{
		Transform inv;

		inv.rotation = this->rotation.inverse();

		inv.scale.x = fabsf(scale.x) < FLOAT_EPSILON ? 0.0f : 1.0f / scale.x;
		inv.scale.y = fabsf(scale.y) < FLOAT_EPSILON ? 0.0f : 1.0f / scale.y;
		inv.scale.z = fabsf(scale.z) < FLOAT_EPSILON ? 0.0f : 1.0f / scale.z;

		Vector3 invTrans = position * -1.0f;

		inv.position = inv.rotation.rotate(multiplyComponents(inv.scale, invTrans));

		return inv;
	}

	__forceinline Matrix4 toMatrix() const noexcept
	{
		Vector3 x = rotation.rotate(Vector3(1, 0, 0));
		Vector3 y = rotation.rotate(Vector3(0, 1, 0));
		Vector3 z = rotation.rotate(Vector3(0, 0, 1));

		x *= scale.x;
		y *= scale.y;
		z *= scale.z;

		return Matrix4(x.x, x.y, x.z, 0, y.x, y.y, y.z, 0, z.x, z.y, z.z, 0, position.x, position.y, position.z, 1);
	}

	__forceinline static Transform fromMatrix(const Matrix4 &m) noexcept
	{
		Transform out;

		out.position = Vector3(m.v[12], m.v[13], m.v[14]);
		out.rotation = Quaternion::fromMatrix(m);

		Matrix4 rotScaleMatrix(m.v[0], m.v[1], m.v[2], 0, m.v[4], m.v[5], m.v[6], 0, m.v[8], m.v[9], m.v[10], 0, 0, 0,
							   0, 1);

		Matrix4 invRotationMatrix = out.rotation.inverse().toMatrix4();
		Matrix4 scaleSkewMatrix = rotScaleMatrix * invRotationMatrix;

		out.scale.x = scaleSkewMatrix.v[0];
		out.scale.y = scaleSkewMatrix.v[5];
		out.scale.z = scaleSkewMatrix.v[10];

		return out;
	}

	__forceinline static Transform mix(const Transform &a, const Transform &b, float t) noexcept
	{
		Quaternion bRotation = b.rotation;
		if (a.rotation.dot(b.rotation) < 0.0f)
		{
			bRotation *= -1;
		}

		return Transform(Vector3::lerp(a.position, b.position, t), Quaternion::nlerp(a.rotation, bRotation, t),
						 Vector3::lerp(a.scale, b.scale, t));
	}

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

#endif // VENGINE3D_VE_TRANSFORM_H
