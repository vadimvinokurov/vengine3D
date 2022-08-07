//
// Created by boris on 8/7/2022.
//

#include "ECSTest.h"
#include <iostream>
static constexpr auto PAYLOAD_SIZE = 9;
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
			assert(c == testval && "Object data destroy");
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
			assert(c == testval && "Object data destroy");
		}
	}
	int32 payload[PAYLOAD_SIZE];
	int32 testval;
};

struct Dwarf : public Entity<Dwarf>
{
	Dwarf()
	{
		testval = rand();
		for (auto &c : payload)
		{
			c = testval;
		}
	}
	~Dwarf()
	{
		for (auto &c : payload)
		{
			assert(c == testval && "Object data destroy");
		}
	}
	int32 payload[PAYLOAD_SIZE];
	int32 testval;
};

ECSTest::ECSTest()
{
	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>(componentManager.get());

	EntityId DragonId = entityManager->createEntity<Dragon>();
	EntityId HumanId = entityManager->createEntity<Human>();
	EntityId Human2Id = entityManager->createEntity<Human>();
	EntityId Human3Id = entityManager->createEntity<Human>();
	EntityId DwarfId = entityManager->createEntity<Dwarf>();


	auto DragonPtr = entityManager->getEntityById(DragonId);
	auto HumanPtr = entityManager->getEntityById(HumanId);
	auto Human2Ptr = entityManager->getEntityById(Human2Id);
	auto Human3Ptr = entityManager->getEntityById(Human3Id);
	auto DwarfPtr = entityManager->getEntityById(DwarfId);

	std::cout << std::hex << DragonId << " " << HumanId << " " << DwarfId << " " << std::endl;
	std::cout << std::dec << sizeof(Dragon) << " " << sizeof(Human) << " " << sizeof(Dwarf) << " " << std::endl;
	std::cout << std::hex << DragonPtr << " " << HumanPtr << " " << Human2Ptr << " " << Human3Ptr << " " << DwarfPtr
			  << " " << std::endl;
	std::cout << std::dec << ((uptr)Human2Ptr - (uptr)HumanPtr) << std::endl;
	std::cout << std::dec << ((uptr)Human3Ptr - (uptr)Human2Ptr) << std::endl;
}

void ECSTest::run()
{
	std::cout << "ecs test" << std::endl;
}
