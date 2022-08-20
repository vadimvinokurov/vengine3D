//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"
#include "Components/StaticMeshComponent.h"

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

	void jump()
	{
		auto staticMesh1 = std::make_shared<StaticMesh>();
		getComponent<StaticMeshComponent>()->setStaticMesh(staticMesh1);
		spdlog::info("Jump");
	}

	void move(float amount)
	{
		spdlog::info("move {}", amount);
	}

	void right(float amount)
	{
		spdlog::info("right {}", amount);
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
	inputComponents->bindAction("Jump", KeyState::PRESSED, dragon, &Dragon::jump);
	inputComponents->bindAxis("MoveForward", dragon, &Dragon::move);
	inputComponents->bindAxis("MoveRight", dragon, &Dragon::right);

	StaticMeshComponent * staticMeshComponent = dragon->addComponent<StaticMeshComponent>();
	auto staticMesh1 = std::make_shared<StaticMesh>();
	auto material1 = std::make_shared<Material>();
	staticMeshComponent->setStaticMesh(staticMesh1);
	staticMeshComponent->setMaterial(material1);
}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
}
void World::onQuite()
{
	VEngine::onQuite();
}
