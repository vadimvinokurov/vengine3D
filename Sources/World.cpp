//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"

struct Dragon : public Entity<Dragon>
{
	Dragon()
	{
	}
	~Dragon()
	{
	}
};




World::World()
{
}

void World::onCreate()
{
	VEngine::onCreate();

	Dragon* dragon = entityManager->createEntity<Dragon>();
	dragon->addComponent<InputComponents>();





}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
}
void World::onQuite()
{
	VEngine::onQuite();
}
