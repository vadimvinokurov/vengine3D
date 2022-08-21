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

class SkyBox : public Entity
{
	GENERATE_ENTITY_BODY();
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

	CameraComponent *cameraComponent = dragon->addComponent<CameraComponent>();
	inputComponents->bindAxis("MoveForward", cameraComponent, &CameraComponent::moveAlongDirection);
	inputComponents->bindAxis("MoveRight", cameraComponent, &CameraComponent::moveAlongSide);

	SkyBox *skyBox = entityManager->createEntity<SkyBox>();

	AssetImporter fbx = AssetImporter("E:\\Work\\vengine3D\\Content\\Mesh\\droid\\source\\droid.blend");
	auto staticMeshs = fbx.loadMeshes();
	AssetImporter sky = AssetImporter("E:\\Work\\vengine3D\\Content\\Mesh\\skysphere\\skysphere.fbx");
	auto skySphere = sky.loadMeshes();

	StaticMeshComponent *staticMeshComponent1 = dragon->addComponent<StaticMeshComponent>();
	staticMeshComponent1->setStaticMesh(std::make_shared<StaticMesh>(staticMeshs[0]));
	staticMeshComponent1->setMaterial(
		std::make_shared<Material>("E:\\Work\\vengine3D\\Content\\Mesh\\droid\\textures\\01___Def.jpeg"));

	StaticMeshComponent *staticMeshComponent2 = skyBox->addComponent<StaticMeshComponent>();
	staticMeshComponent2->setStaticMesh(std::make_shared<StaticMesh>(skySphere[0]));
	staticMeshComponent2->setMaterial(
		std::make_shared<Material>("E:\\Work\\vengine3D\\Content\\Mesh\\skysphere\\sky_texture.jpg"));
	staticMeshComponent2->transform.scale = Vector3(4000.0f);
}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
}
void World::onQuite()
{
	VEngine::onQuite();
}
