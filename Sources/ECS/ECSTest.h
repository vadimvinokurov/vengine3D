//
// Created by boris on 8/7/2022.
//

#ifndef VENGINE3D_ECSTEST_H
#define VENGINE3D_ECSTEST_H

#include "ECS/Managers/ComponentManager.h"
#include "ECS/Managers/EntityManager.h"
#include "ECS/Managers/SystemManager.h"

class ECSTest
{
public:
	ECSTest();
	void run();

private:
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;
	std::vector<IEntity *> entities;
};

#endif // VENGINE3D_ECSTEST_H
