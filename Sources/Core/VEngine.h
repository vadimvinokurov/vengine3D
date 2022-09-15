//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_VENGINE_H
#define VENGINE3D_VENGINE_H

#include "VEngineBase.h"

struct VEngine
{
	static void initialize();
	static void destroy();
	static Window *getWindow();
	static World *getWorld();
	static void run();

private:
	static std::unique_ptr<VEngineBase> engineInstance_;
}; // namespace VEngine

#endif // VENGINE3D_VENGINE_H
