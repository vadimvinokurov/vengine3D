//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_POOLALLOCATOR_H
#define VENGINE3D_POOLALLOCATOR_H

#include "IAllocator.h"

class PoolAllocator : public IAllocator
{
public:
	PoolAllocator(void *memory, size_t size, size_t objectSize, uint8 objectAlignment);
	void *allocate(size_t size, uint8 alignment) override;
	void deallocate(void *ptr) override;
	void clear() override;

private:
	const size_t OBJECT_SIZE;
	const size_t OBJECT_ALIGNMENT;

	void **nextFreeBlock = nullptr;
};

#endif // VENGINE3D_POOLALLOCATOR_H
