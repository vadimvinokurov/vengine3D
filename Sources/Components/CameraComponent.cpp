//
// Created by boris on 8/20/2022.
//

#include "CameraComponent.h"
Matrix4 CameraComponent::getViewMatrix() const
{
	const Vector3 &R = right_;
	const Vector3 &U = up_;
	const Vector3 &D = direction_;
	const Vector3 &P = transform.position;

	// clang-format off
	return Matrix4(R.x, U.x, D.x, P.x,
				   R.y, U.y, D.y, P.y,
				   R.z, U.z, D.z, P.z,
				   0.0f, 0.0f, 0.0f, 1.0f).getInversed();
	// clang-format on
}

void CameraComponent::moveAlongDirection(float amount)
{
	transform.position += direction_ * amount * -0.1f;
}

void CameraComponent::moveAlongSide(float amount)
{
	transform.position += right_ * amount * 0.1f;
}

Matrix4 CameraComponent::perspective(float fov, float aspect, float n, float f)
{
	float ymax = n * tanf(fov * static_cast<float>(VEMath_PI) / 360.0f);
	float xmax = ymax * aspect;
	return CameraComponent::frustum(-xmax, xmax, -ymax, ymax, n, f);
}

Matrix4 CameraComponent::frustum(float left, float right, float bottom, float top, float n, float f)
{
	if (left == right || top == bottom || n == f)
	{
		assert(false && "Invalid frustum\n");
		return Matrix4();
	}

	// clang-format off
	return Matrix4(
			(2.0f * n) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
			0.0f, (2.0f * n) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
			0.0f, 	0.0f, 	(-(f + n)) / (f - n), (-2.0f * f * n) / (f - n),
			0.0f, 0.0f, -1.0f, 	0.0f);
	// clang-format on
}