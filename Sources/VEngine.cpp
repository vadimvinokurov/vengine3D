//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Window.h"
#include "ECS/ECS.h"
#include "Systems/RenderSystem.h"

VEngine::VEngine()
{
	window_ = std::make_unique<Window>(windowDefaultWidth_, windowDefaultHeight_);
	window_->makeContextCurrent();
	ecs = std::make_unique<ECS>();
	return;
}

void VEngine::run()
{
	onCreate();

	while (!window_->shouldClose())
	{
		auto frameStart = std::chrono::steady_clock::now();
		Window::poolEvents();
		ecs->update(deltaTime_);
		onUpdate();
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
	RenderSystem* renderSystem = ecs->systemManager->addSystem<RenderSystem>();
	window_->OnWindowResizeDelegate.connect(renderSystem, &RenderSystem::resize);
}

void VEngine::onUpdate()
{
	//renderEngine_->clear();
	//renderEngine_->update();

}

void VEngine::onQuite()
{
}
