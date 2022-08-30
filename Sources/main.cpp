//
// Created by boris on 7/6/2022.
//
#include "spdlog/spdlog.h"
#include "World.h"

int main()
{
	SPDLOG_INFO("Start engine!");
	getWorld()->run();
	return 0;
}