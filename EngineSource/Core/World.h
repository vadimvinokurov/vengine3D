//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include "Core/Objects/Entity.h"
#include "Core/Managers/SystemManager.h"
#include "Core/Managers/ComponentManager.h"
#include "Core/Managers/EntityManager.h"

class SkySphere;
class World
{
public:
	World();
	virtual ~World() = default;
	virtual void onCreate();
	virtual void update(float dt);
	virtual void onQuite();

	Entity *getEntityByEntityId(EntityId entityId)
	{
		return entityManager->getEntityPtr(entityId);
	}

	template <class T>
	std::pair<ComponentManager::ComponentIterator<T>, ComponentManager::ComponentIterator<T>> getComponents()
	{
		return std::pair<ComponentManager::ComponentIterator<T>, ComponentManager::ComponentIterator<T>>(
			componentManager->begin<T>(), componentManager->end<T>());
	}

protected:
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;

	SkySphere *skySphere;
};

#endif // VENGINE3D_WORLD_H
