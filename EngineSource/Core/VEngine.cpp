//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Render/opengl_glad.h"

std::unique_ptr<VEngineBase> VEngine::engineInstance_;

void VEngine::initialize()
{
	assert(engineInstance_.get() == nullptr && "engineInstance_ already initialized");
	engineInstance_ = std::make_unique<VEngineBase>();
	initOpenGl();
	SPDLOG_INFO("Engine has been initialized!");
}

void VEngine::destroy()
{
	engineInstance_.reset();
	SPDLOG_INFO("Engine has been destroyed!");
}

void VEngine::run()
{
	engineInstance_->run();
}

Window *VEngine::getWindow()
{
	return engineInstance_->getWindow();
}

World *VEngine::getWorld()
{
	return engineInstance_->getWorld();
}

void VEngine::setWorld(const std::string &name)
{
	engineInstance_->setWorld(name);
}