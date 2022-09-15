//
// Created by boris on 15.09.2022.
//

#include "VEngineBase.h"
#include "World.h"

VEngineBase::VEngineBase()
{
	window_ = std::make_unique<Window>(windowDefaultWidth_, windowDefaultHeight_);
	window_->makeContextCurrent();
}
VEngineBase::~VEngineBase() = default;

void VEngineBase::run()
{
	world_ = std::make_unique<World>();
	while (!window_->shouldClose())
	{
		auto frameStart = std::chrono::steady_clock::now();
		Window::poolEvents();
		world_->update(deltaTime_);
		window_->swapBuffer();

		std::chrono::duration<double> target(deltaTime_);
		while (std::chrono::steady_clock::now() - frameStart < target)
		{
			std::this_thread::yield();
		}
	}
}

Window *VEngineBase::getWindow()
{
	return window_.get();
}

World *VEngineBase::getWorld()
{
	return world_.get();
}
