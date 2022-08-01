//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_POOLALLOCATOR_H
#define VENGINE3D_POOLALLOCATOR_H

#include "IAllocator.h"
#include "MemoryPool.h"

class PoolAllocator : public IAllocator
{
public:
	PoolAllocator(MemoryPool&& memoryPool, size_t objectSize, uint8 objectAlignment);
	void *allocate(size_t = 1, uint8 = 1) override;
	void free(void *ptr) override;
	bool own(void *ptr) const override;

private:
	void clear();
	const size_t OBJECT_SIZE;
	const size_t OBJECT_ALIGNMENT;
	MemoryPool&& memoryPool_;
	void **nextFreeBlock = nullptr;
};

#endif // VENGINE3D_POOLALLOCATOR_H
