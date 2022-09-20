//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_STACKALLOCATOR_H
#define VENGINE3D_STACKALLOCATOR_H

#include "IAllocator.h"
#include "Core/Memory/MemoryPool.h"

#include <vector>

class StackAllocator final : public IAllocator
{
private:
	struct MetaInfo
	{
		uint8 adjustment;
	};

public:
	static AllocatorPtr create(MemoryPoolPtr memoryPool)
	{
		return AllocatorPtr(new StackAllocator(std::move(memoryPool)));
	};
	void *allocate(size_t size, uint8 alignment) override;
	void free(void *ptr) override;
	bool own(void *ptr) const override;

private:
	explicit StackAllocator(MemoryPoolPtr memoryPool);
	void *allocate_memory(size_t size, uint8 alignment);
	void deallocate_memory(void *ptr);

	MemoryPoolPtr memoryPool_;
	std::vector<void *> issuedMemory_;
	std::vector<void *> freedMemory_;
};

#endif // VENGINE3D_STACKALLOCATOR_H
