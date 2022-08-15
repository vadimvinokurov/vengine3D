//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ECS_H
#define VENGINE3D_ECS_H

#include "ECS/Managers/EntityManager.h"
#include "ECS/Managers/ComponentManager.h"
#include "ECS/Managers/SystemManager.h"

struct ECS {
	ECS() {
		systemManager = std::make_unique<SystemManager>();
		componentManager = std::make_unique<ComponentManager>();
		entityManager = std::make_unique<EntityManager>(componentManager.get());
	}
	void update(float dt) {
		systemManager->update(dt);
		entityManager->removeDestroyedEntities();
	}

	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;
};

#endif // VENGINE3D_ECS_H
