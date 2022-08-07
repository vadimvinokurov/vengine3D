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

ECSTest::ECSTest()
{
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

	dragon->addComponent<Movement>("Dragon do fly " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Render>("Render dragon " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Camera>("Dragon Camera " + std::to_string(dragon->getEntityId()));

	dragon->addComponent<Movement>("Dragon do fly " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Render>("Render dragon " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Camera>("Dragon Camera " + std::to_string(dragon->getEntityId()));

	dragon->addComponent<Movement>("Dragon do fly " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Render>("Render dragon " + std::to_string(dragon->getEntityId()));
	dragon->addComponent<Camera>("Dragon Camera " + std::to_string(dragon->getEntityId()));

	componentManager->removeAllComponent(dwarf->getEntityId());

	//dwarf->removeComponent<Render>();

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
