//
// Created by boris on 15.09.2022.
//

#ifndef VENGINE3D_VENGINEBASE_H
#define VENGINE3D_VENGINEBASE_H

#include "EngineCore.h"
#include "World.h"
#include "Window.h"

class VEngineBase final
{
public:
	VEngineBase();
	~VEngineBase();
	void run();
	Window *getWindow();
	World *getWorld();

	template <typename T>
	World *createWorld(const std::string &name);
	void setWorld(const std::string &name);

private:
	constexpr static auto windowDefaultWidth_ = 1280;
	constexpr static auto windowDefaultHeight_ = 720;
	constexpr static auto windowDefaultFps_ = 60;
	constexpr static float deltaTime_ = 1.0f / windowDefaultFps_;

	std::unique_ptr<Window> window_;
	World *currentWorld_ = nullptr;
	World *newCurrentWorld_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<World>> worldPool_;
};

template <typename T>
World *VEngineBase::createWorld(const std::string &name)
{
	auto it = worldPool_.emplace(name, std::make_unique<T>());
	if (!it.second)
	{
		SPDLOG_WARN("World has already been created");
		return nullptr;
	}
	it.first->second->onCreate();
	return it.first->second.get();
}

#endif // VENGINE3D_VENGINEBASE_H
