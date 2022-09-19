//
// Created by boris on 15.09.2022.
//

#include "VEngineBase.h"
#include "Window.h"

VEngineBase::VEngineBase()
{
	window_ = std::make_unique<Window>(windowDefaultWidth_, windowDefaultHeight_);
	window_->makeContextCurrent();
}

void VEngineBase::run()
{
	while (!window_->shouldClose())
	{
		if (newCurrentWorld_)
		{
			currentWorld_ = newCurrentWorld_;
			newCurrentWorld_ = nullptr;
			SPDLOG_INFO("Change world");
		}
		auto frameStart = std::chrono::steady_clock::now();
		Window::poolEvents();
		assert(currentWorld_ && "World is not set!");
		currentWorld_->update(deltaTime_);
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
	return currentWorld_;
}

void VEngineBase::setWorld(const std::string &name)
{
	auto it = worldPool_.find(name);
	if (it == worldPool_.end())
	{
		SPDLOG_ERROR("World not found in pool.");
		std::exit(0);
	}
	newCurrentWorld_ = it->second.get();
	SPDLOG_INFO("Using world: {}", it->first);
}
VEngineBase::~VEngineBase()
{
	for (auto &world : worldPool_)
	{
		world.second->onQuite();
	}
	worldPool_.clear();
}
