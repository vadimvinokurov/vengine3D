//
// Created by boris on 7/6/2022.
//
#include "spdlog/spdlog.h"
#include "Core/VEngine.h"
#include "HaloWorld.h"
#include "AstroWorld.h"

int main()
{
	VEngine::initialize();
	VEngine::createWorld<AstroWorld>("AstroWorld");
	//VEngine::createWorld<HaloWorld>("HaloWorld");
	VEngine::setWorld("AstroWorld");
	VEngine::run();
	VEngine::destroy();
	return 0;
}