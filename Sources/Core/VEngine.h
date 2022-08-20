//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_VENGINE_H
#define VENGINE3D_VENGINE_H

#include "EngineCore.h"
#include "Core/Managers/ComponentManager.h"
#include "Core/Managers/EntityManager.h"
#include "Core/Managers/SystemManager.h"

class Window;

class VEngine
{
public:
	VEngine();
	~VEngine();

	virtual void onCreate();
	virtual void onUpdate(float dt);
	virtual void onQuite();

	void run();

	template <class T>
	std::pair<ComponentManager::ComponentIterator<T>, ComponentManager::ComponentIterator<T>> getComponents()
	{
		return std::pair<ComponentManager::ComponentIterator<T>, ComponentManager::ComponentIterator<T>>(
			componentManager->begin<T>(), componentManager->end<T>());
	}

	Entity* getEntityByEntityId(EntityId entityId);

protected:
	constexpr static auto windowDefaultWidth_ = 1280;
	constexpr static auto windowDefaultHeight_ = 720;
	constexpr static auto windowDefaultFps_ = 60;
	constexpr static float deltaTime_ = 1.0f / windowDefaultFps_;

	std::unique_ptr<Window> window_;
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;
};

#endif // VENGINE3D_VENGINE_H
