//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CameraComponent.h"

class Dragon : public Entity
{
	GENERATE_ENTITY_BODY()
public:
	Dragon()
	{
	}
	~Dragon()
	{
	}

	void cameraLock()
	{
		cameraLockFlag = true;
	}

	void cameraUnlock()
	{
		cameraLockFlag = false;
	}

	void move(float amount)
	{
		spdlog::info("move {}", amount);
	}

	void right(float amount)
	{
		spdlog::info("right {}", amount);
	}

	void lookUp(float amount)
	{
		if (amount == 0.0f || cameraLockFlag == false)
		{
			return;
		}
		getComponent<CameraComponent>()->addPitchInput(amount);
	}

	void turn(float amount)
	{
		if (amount == 0.0f || cameraLockFlag == false)
		{
			return;
		}
		getComponent<CameraComponent>()->addYawInput(amount);
	}

	bool cameraLockFlag = false;
};

World::World()
{
}

void World::onCreate()
{
	VEngine::onCreate();

	Dragon *dragon = entityManager->createEntity<Dragon>();
	InputComponents *inputComponents = dragon->addComponent<InputComponents>();
	inputComponents->bindAction("CameraLock", KeyState::PRESSED, dragon, &Dragon::cameraLock);
	inputComponents->bindAction("CameraLock", KeyState::RELEASE, dragon, &Dragon::cameraUnlock);
	inputComponents->bindAxis("Turn", dragon, &Dragon::turn);
	inputComponents->bindAxis("LookUp", dragon, &Dragon::lookUp);

	StaticMeshComponent *staticMeshComponent = dragon->addComponent<StaticMeshComponent>();
	auto staticMesh1 = std::make_shared<StaticMesh>();
	auto material1 = std::make_shared<Material>();
	staticMeshComponent->setStaticMesh(staticMesh1);
	staticMeshComponent->setMaterial(material1);

	CameraComponent *cameraComponent = dragon->addComponent<CameraComponent>();
	inputComponents->bindAxis("MoveForward", cameraComponent, &CameraComponent::moveAlongDirection);
	inputComponents->bindAxis("MoveRight", cameraComponent, &CameraComponent::moveAlongSide);
}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
}
void World::onQuite()
{
	VEngine::onQuite();
}
