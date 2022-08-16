//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"

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
}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
}
void World::onQuite()
{
	VEngine::onQuite();
}
