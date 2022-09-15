//
// Created by boris on 15.09.2022.
//

#ifndef VENGINE3D_VENGINEBASE_H
#define VENGINE3D_VENGINEBASE_H

#include "EngineCore.h"
#include "Window.h"

class World;
class VEngineBase
{
public:
	VEngineBase();
	~VEngineBase();
	void run();
	Window *getWindow();
	World *getWorld();

private:
	constexpr static auto windowDefaultWidth_ = 1280;
	constexpr static auto windowDefaultHeight_ = 720;
	constexpr static auto windowDefaultFps_ = 60;
	constexpr static float deltaTime_ = 1.0f / windowDefaultFps_;

	std::unique_ptr<Window> window_;
	std::unique_ptr<World> world_;
};

#endif // VENGINE3D_VENGINEBASE_H
