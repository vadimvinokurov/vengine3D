//
// Created by boris on 7/6/2022.
//

#include <VEngine.h>
#include <iostream>
#include "ECS/Memory/MemoryManager.h"
int main()
{
	MemoryManager memoryManager;
	auto a = memoryManager.allocateMemoryPool(100);
	VEngine vengine;
	vengine.run();
	return 0;
}