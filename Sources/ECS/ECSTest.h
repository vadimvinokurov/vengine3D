//
// Created by boris on 8/7/2022.
//

#ifndef VENGINE3D_ECSTEST_H
#define VENGINE3D_ECSTEST_H

#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"

class ECSTest
{
public:
	ECSTest();
	void run();

private:
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<EntityManager> entityManager;
	std::vector<EntityId> entities;
};

#endif // VENGINE3D_ECSTEST_H
