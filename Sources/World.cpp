//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CameraComponent.h"
#include "AssetImporter.h"

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
};

World::World()
{
}

void World::onCreate()
{
	VEngine::onCreate();

	Dragon *dragon = entityManager->createEntity<Dragon>();
	InputComponents *inputComponents = dragon->addComponent<InputComponents>();
	inputComponents->bindAxis("Turn", dragon, &Dragon::turn);
	inputComponents->bindAxis("LookUp", dragon, &Dragon::lookUp);

	StaticMeshComponent *staticMeshComponent = dragon->addComponent<StaticMeshComponent>();

	AssetImporter fbx = AssetImporter("e:\\Work\\vengine3D\\Content\\Mesh\\uechar.FBX");


	StaticMesh staticMesh = fbx.loadMeshes().front();
	auto staticMeshPtr = std::make_shared<StaticMesh>(staticMesh);
	auto material1 = std::make_shared<Material>();
	staticMeshComponent->setStaticMesh(staticMeshPtr);
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
