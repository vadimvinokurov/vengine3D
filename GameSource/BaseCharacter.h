//
// Created by boris on 19.09.2022.
//

#ifndef VENGINE3D_BASECHARACTER_H
#define VENGINE3D_BASECHARACTER_H

#include "Core/Objects/Entity.h"
#include "Components/CameraComponent.h"
#include "Components/InputComponents.h"
#include "Components/SkeletalMeshComponent.h"

class BaseCharacter : public Entity
{
	GENERATE_ENTITY_BODY(Entity)
public:
	void lookUp(float amount)
	{
		if (amount == 0.0f)
		{
			return;
		}
		getComponent<CameraComponent>()->addPitchInput(amount);
	}

	void turn(float amount)
	{
		if (amount == 0.0f)
		{
			return;
		}
		getComponent<CameraComponent>()->addYawInput(amount);
	}

	void onCreate() override{
		Super::onCreate();

		auto inputComponents = addComponent<InputComponents>();
		inputComponents->bindAxis("Turn", this, &BaseCharacter::turn);
		inputComponents->bindAxis("LookUp", this, &BaseCharacter::lookUp);

		auto cameraComponent = addComponent<CameraComponent>();
		inputComponents->bindAxis("MoveForward", cameraComponent, &CameraComponent::moveAlongDirection);
		inputComponents->bindAxis("MoveRight", cameraComponent, &CameraComponent::moveAlongSide);

		skeletalMeshComponent = addComponent<SkeletalMeshComponent>();
	}

	SkeletalMeshComponent *skeletalMeshComponent;
};


#endif // VENGINE3D_BASECHARACTER_H
