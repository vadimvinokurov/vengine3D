//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_CAMERACOMPONENT_H
#define VENGINE3D_CAMERACOMPONENT_H

#include "Core/Objects/TransformComponent.h"
#include "Math/MathConstant.h"

class CameraComponent : public TransformComponent
{
public:
	CameraComponent() = default;

	Matrix4 getViewMatrix() const;

	void moveAlongDirection(float amount);

	void moveAlongSide(float amount);

	static Matrix4 perspective(float fov, float aspect, float n, float f);

	static Matrix4 frustum(float left, float right, float bottom, float top, float n, float f);

	void addPitchInput(float val);

	void addYawInput(float val);


private:
	static constexpr float pitchScale = -0.3f;
	static constexpr float yawScale = -0.3f;
	Vector3 direction_ = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 up_ = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 right_ = Vector3(1.0f, 0.0f, 0.0f);
};

#endif // VENGINE3D_CAMERACOMPONENT_H
