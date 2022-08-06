//
// Created by boris on 7/6/2022.
//

#include <VEngine.h>
#include "ECS/EntityManager.h"

int main()
{
	class A : public Entity<A>
	{
		uint8 payload[8];
	};

	EntityManager entityManager;

	auto entityId = entityManager.createEntity<A>();
	entityManager.destroyEntity(entityId);
	entityManager.removeDestroyedEntities();
	//std::cout << entityId.value << std::endl;
	VEngine vengine;
	vengine.run();
	return 0;
}