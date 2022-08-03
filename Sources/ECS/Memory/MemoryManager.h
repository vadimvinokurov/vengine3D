//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_MEMORYMANAGER_H
#define VENGINE3D_MEMORYMANAGER_H

#include "StackAllocator.h"
#include <list>
#include <string>

class MemoryManager: public IAllocator
{
public:
	MemoryManager();
	void *allocate(size_t size, uint8 = 1) override;
	void free(void *ptr) override;
	virtual bool own(void *ptr) const override;

private:
	static constexpr size_t CHUNK_SIZE = 256 * 1024 * 1024ull;
	std::list<StackAllocator> memoryChunks_;
};

#endif // VENGINE3D_MEMORYMANAGER_H
