//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Window.h"
#include "Systems/RenderSystem.h"
#include "Systems/ControllerSystem.h"

VEngine::VEngine()
{
	window_ = std::make_unique<Window>(windowDefaultWidth_, windowDefaultHeight_);
	window_->makeContextCurrent();
	systemManager = std::make_unique<SystemManager>();
	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>(componentManager.get());
	return;
}

void VEngine::run()
{
	onCreate();

	while (!window_->shouldClose())
	{
		auto frameStart = std::chrono::steady_clock::now();
		Window::poolEvents();
		systemManager->update(deltaTime_);
		entityManager->removeDestroyedEntities();
		onUpdate(deltaTime_);
		window_->swapBuffer();

		std::chrono::duration<double> target(deltaTime_);
		while (std::chrono::steady_clock::now() - frameStart < target)
		{
			std::this_thread::yield();
		}
	}
	onQuite();
}
VEngine::~VEngine()
{
}

void VEngine::onCreate()
{
	ControllerSystem *controllerSystem = systemManager->addSystem<ControllerSystem>(System::HIGHEST_SYSTEM_PRIORITY);
	window_->onKeyboardKeyDelegate.connect(controllerSystem, &ControllerSystem::onKeyboardKey);
	window_->onMouseKeyDelegate.connect(controllerSystem, &ControllerSystem::onMouseKey);
	window_->onMousePositionDelegate.connect(controllerSystem, &ControllerSystem::onMousePosition);

	RenderSystem *renderSystem = systemManager->addSystem<RenderSystem>(System::LOWEST_SYSTEM_PRIORITY);
	window_->onWindowResizeDelegate.connect(renderSystem, &RenderSystem::resize);
}

void VEngine::onUpdate(float dt)
{
}

void VEngine::onQuite()
{
}
