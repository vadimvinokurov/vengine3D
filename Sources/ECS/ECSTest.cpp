//
// Created by boris on 8/7/2022.
//

#include "ECSTest.h"
#include <iostream>
static constexpr auto PAYLOAD_SIZE = 10;

struct Dragon : public Entity<Dragon>
{
	Dragon()
	{
		testval = rand();
		for (auto &c : payload)
		{
			c = testval;
		}
	}
	~Dragon()
	{
		for (auto &c : payload)
		{
			assert(c == testval && "Object data corrupted");
		}
	}
	int32 payload[PAYLOAD_SIZE];
	int32 testval;
};

struct Human : public Entity<Human>
{
	Human()
	{
		testval = rand();
		for (auto &c : payload)
		{
			c = testval;
		}
	}
	~Human()
	{
		for (auto &c : payload)
		{
			assert(c == testval && "Object data corrupted");
		}
	}
	int32 payload[PAYLOAD_SIZE + 3];
	int32 testval;
};

struct Dwarf : public Entity<Dwarf>
{
	Dwarf(int32 value)
	{
		val = value;
		testval = rand();
		for (auto &c : payload)
		{
			c = testval  ^ val;
		}
	}
	~Dwarf()
	{
		for (auto &c : payload)
		{
			assert((c ^ val) == testval && "Object data corrupted");
		}
	}
	int32 payload[PAYLOAD_SIZE - 4];
	int32 testval;
	int32 val;
};

ECSTest::ECSTest()
{
	std::cout << std::dec << sizeof(Dragon) << " " << sizeof(Human) << " " << sizeof(Dwarf) << " " << std::endl;


	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>(componentManager.get());
	std::cout << "Generate Entities" << std::endl;
	for (int i = 0; i < 1000; ++i)
	{
		entities.push_back(entityManager->createEntity<Dragon>());
		entities.push_back(entityManager->createEntity<Dwarf>(10));
		entities.push_back(entityManager->createEntity<Human>());
		entityManager->destroyEntity(entities.back());
	}
	entityManager->removeDestroyedEntities();
	std::cout << "Generated Entities" << std::endl;


	//	for (auto entity : entities)
	//	{
	//		std::cout << std::hex << entityManager->getEntityById(entity) << std::endl;
	//	}

	//	std::cout << std::hex << DragonId << " " << HumanId << " " << DwarfId << " " << std::endl;
	//	std::cout << std::dec << sizeof(Dragon) << " " << sizeof(Human) << " " << sizeof(Dwarf) << " " << std::endl;
	//	std::cout << std::hex << DragonPtr << " " << HumanPtr << " " << Human2Ptr << " " << Human3Ptr << " " << DwarfPtr
	//			  << " " << std::endl;
	//	std::cout << std::dec << ((uptr)Human2Ptr - (uptr)HumanPtr) << std::endl;
	//	std::cout << std::dec << ((uptr)Human3Ptr - (uptr)Human2Ptr) << std::endl;
}

void ECSTest::run()
{
	std::cout << "ecs test" << std::endl;
}
