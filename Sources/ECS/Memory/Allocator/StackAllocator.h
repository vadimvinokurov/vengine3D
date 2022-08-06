//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_STACKALLOCATOR_H
#define VENGINE3D_STACKALLOCATOR_H

#include "IAllocator.h"
#include "ECS/Memory/MemoryPool.h"

#include <vector>

class StackAllocator final : public IAllocator
{
private:
	struct MetaInfo
	{
		uint8 adjustment;
	};

public:
	StackAllocator(const std::shared_ptr<MemoryPool> &memoryPool);
	void *allocate(size_t size, uint8 alignment) override;
	void free(void *ptr) override;
	bool own(void *ptr) const override;

private:
	void *allocate_memory(size_t size, uint8 alignment);
	void deallocate_memory(void *ptr);

	std::shared_ptr<MemoryPool> memoryPool_;
	std::vector<void *> issuedMemory_;
	std::vector<void *> freedMemory_;
};

#endif // VENGINE3D_STACKALLOCATOR_H
