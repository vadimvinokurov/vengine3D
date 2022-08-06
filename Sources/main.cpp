//
// Created by boris on 7/6/2022.
//

#include <VEngine.h>
#include "ECS/EntityManager.h"

int main()
{
	struct A: public Entity<A> {
		uint8 c[8];
	};
	struct B: public Entity<A> {
		uint8 c[8];
	};

	EntityManager man;
	auto entityId = man.createEntity<A>();
	man.destroyEntity(entityId);
	man.removeDestroyedEntities();



	VEngine vengine;
	vengine.run();
	return 0;
}