//
// Created by boris on 7/6/2022.
//
#include "spdlog/spdlog.h"
#include "Core/VEngine.h"

int main()
{
	VEngine::initialize();
	VEngine::run();
	VEngine::destroy();
	return 0;
}