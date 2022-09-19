//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_VENGINE_H
#define VENGINE3D_VENGINE_H

#include <string>
#include <memory>
#include "VEngineBase.h"

struct VEngine
{
	static void initialize();
	static void destroy();
	static Window *getWindow();
	static World *getWorld();
	template <typename T>
	static void createWorld(const std::string &name);
	static void setWorld(const std::string &name);
	static void run();

private:
	static std::unique_ptr<VEngineBase> engineInstance_;

}; // namespace VEngine

template <typename T>
void VEngine::createWorld(const std::string &name)
{
	engineInstance_->createWorld<T>(name);
}

#endif // VENGINE3D_VENGINE_H
