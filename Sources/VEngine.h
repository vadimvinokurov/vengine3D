//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_VENGINE_H
#define VENGINE3D_VENGINE_H

#include "EngineCore.h"

class MemoryManager;
class RenderEngine;
class Window;

class VEngine
{
public:
	VEngine();
	~VEngine();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onQuite();

	void run();

protected:
	constexpr static auto windowDefaultWidth_ = 1280;
	constexpr static auto windowDefaultHeight_ = 720;
	constexpr static auto windowDefaultFps_ = 60;
	constexpr static float deltaTime_ = 1.0f / windowDefaultFps_;

	std::unique_ptr<Window> window_;
	std::unique_ptr<RenderEngine> renderEngine_;
	std::unique_ptr<MemoryManager> memoryManager_;
};

#endif // VENGINE3D_VENGINE_H
