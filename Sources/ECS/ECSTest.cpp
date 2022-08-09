//
// Created by boris on 8/7/2022.
//

#include "ECSTest.h"
#include <iostream>
#include "ECS/Memory/Allocator/LinearAllocator.h"
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
			c = testval ^ val;
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

struct Movement : public Component<Movement>
{
	Movement(const std::string &action) : action_(action)
	{
		// std::cout << "Create Movement component" << std::endl;
	}
	~Movement()
	{
		// std::cout << "Delete Movement component" << std::endl;
	}
	void update()
	{
		std::cout << action_ << std::endl;
	};
	std::string action_;
};

struct Render : public Component<Render>
{
	Render(const std::string &action) : action_(action)
	{
		// std::cout << "Create Render component" << std::endl;
	}
	~Render()
	{
		// std::cout << "Delete Render component" << std::endl;
	}
	void update()
	{
		std::cout << action_ << std::endl;
	};
	std::string action_;
};

struct Camera : public Component<Camera>
{
	Camera(const std::string &action) : action_(action)
	{
		// std::cout << "Create Camera component" << std::endl;
	}
	~Camera()
	{
		// std::cout << "Delete Camera component" << std::endl;
	}
	void update()
	{
		std::cout << action_ << std::endl;
	};
	std::string action_;
};

struct RenderSystem : public System<RenderSystem>
{
	virtual void update(float dt) override
	{
		std::cout << "Render some scene" << std::endl;
	}
};
struct PhysxSystem : public System<PhysxSystem>
{
	virtual void update(float dt) override
	{
		std::cout << "PhysxSystem some scene" << std::endl;
	}
};

ECSTest::ECSTest()
{
	systemManager = std::make_unique<SystemManager>();
	auto render = systemManager->addSystem<RenderSystem>();
	auto physx = systemManager->addSystem<RenderSystem>();
	render->update(10);
	physx->update(10);
	return;
	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>(componentManager.get());

	for (int i = 0; i < 1000; ++i)
	{
		entities.push_back(entityManager->createEntity<Dragon>());
		entities.push_back(entityManager->createEntity<Dwarf>(10));
		entities.push_back(entityManager->createEntity<Human>());
		entityManager->destroyEntity(entities.back()->getEntityId());
	}
	entityManager->removeDestroyedEntities();
	entities.clear();

	auto dragon = entityManager->createEntity<Dragon>();
	auto dwarf = entityManager->createEntity<Dwarf>(10);
	auto human = entityManager->createEntity<Human>();

	dragon->addComponent<Movement>("Dragon do fly 1 " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Render>("Dragon do fly 2 " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Camera>("Dragon do fly 3 " + std::to_string(dragon->getEntityId()));

	dwarf->addComponent<Movement>("dwarf do fly 1 " + std::to_string(dwarf->getEntityId()));
	dwarf->addComponent<Render>("dwarf do fly 2 " + std::to_string(dwarf->getEntityId()));
	dwarf->addComponent<Camera>("dwarf do fly 3 " + std::to_string(dwarf->getEntityId()));

	human->addComponent<Movement>("human do fly 1 " + std::to_string(human->getEntityId()));
	human->addComponent<Render>("human do fly 2 " + std::to_string(human->getEntityId()));
	human->addComponent<Camera>("human do fly 3 " + std::to_string(human->getEntityId()));

	// componentManager->removeAllComponent(dragon->getEntityId());

	// dwarf->removeComponent<Render>();

	for (auto it = componentManager->begin<Render>(); it != componentManager->end<Render>(); ++it)
	{
		it->update();
	}
	std::cout << std::endl;
	for (auto it = componentManager->begin<Movement>(); it != componentManager->end<Movement>(); ++it)
	{
		it->update();
	}
	std::cout << std::endl;
	for (auto it = componentManager->begin<Camera>(); it != componentManager->end<Camera>(); ++it)
	{
		it->update();
	}
}

void ECSTest::run()
{
}
