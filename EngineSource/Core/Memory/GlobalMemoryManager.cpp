//
// Created by boris on 8/3/2022.
//

#include "GlobalMemoryManager.h"
#include "EngineProperty.h"

AllocatorPtr GlobalMemoryManager::memoryManager =
	ChunkAllocator<StackAllocator>::create(SystemAllocator::create(), GLOBAL_MEMORY_MANAGER_CHUNK_SIZE, 1);
