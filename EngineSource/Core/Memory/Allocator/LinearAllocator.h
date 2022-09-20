//
// Created by boris on 8/9/2022.
//

#ifndef VENGINE3D_LINEARALLOCATOR_H
#define VENGINE3D_LINEARALLOCATOR_H

#include "Core/Memory/Allocator/IAllocator.h"
#include "Core/Memory/MemoryPool.h"
#include "Core/Memory/MemoryUtils.h"

class LinearAllocator final : public IAllocator
{
public:
	static AllocatorPtr create(MemoryPoolPtr memoryPool)
	{
		return AllocatorPtr(new LinearAllocator(std::move(memoryPool)));
	};

	void *allocate(size_t size, uint8 alignment) override
	{
		assert(size > 0 && alignment > 0 && "Allocate called with size = 0 or alignment = 0.");
		if (size == 0 || alignment == 0)
		{
			return nullptr;
		}

		union {
			void *asVoid;
			uptr asUptr;
		};
		asVoid = memoryPool_->address;
		asUptr += memoryPool_->used;

		auto adjustment = MemoryUtils::AlignAdjustment(asVoid, alignment);
		if (memoryPool_->used + size + adjustment > memoryPool_->size)
		{
			return nullptr;
		}

		asUptr += adjustment;
		memoryPool_->used += size + adjustment;
#ifdef ECS_DEBUG
		memset(asVoid, 0xFF, size);
#endif
		return asVoid;
	}

	void free(void *ptr) override
	{
		assert(false && "This method is not supported.");
	}
	bool own(void *ptr) const override
	{
		return memoryPool_->own(ptr);
	}

private:
	LinearAllocator(MemoryPoolPtr memoryPool) : memoryPool_(std::move(memoryPool)){};
	MemoryPoolPtr memoryPool_;
};
#endif // VENGINE3D_LINEARALLOCATOR_H
