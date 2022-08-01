//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_MEMORYMANAGER_H
#define VENGINE3D_MEMORYMANAGER_H

#include "StackAllocator.h"
#include <list>
#include <string>

class MemoryManager
{
public:
	MemoryManager(size_t chunkSize);
	void *allocate(size_t size, const std::string &tag);
	void deallocate(void *ptr);

private:
	const size_t CHUNK_SIZE;
	std::list<StackAllocator> memoryChunks_;
};

#endif // VENGINE3D_MEMORYMANAGER_H
