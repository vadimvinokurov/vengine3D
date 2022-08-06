//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_BLOCKALLOCATOR_H
#define VENGINE3D_BLOCKALLOCATOR_H

#include "IAllocator.h"
#include "ECS/Memory/MemoryPool.h"
#include "EngineLibs.h"
#include "ECS/Memory/MemoryUtils.h"

template <size_t BLOCK_SIZE, size_t ALIGNMENT>
class BlockAllocator : public IAllocator
{
public:
	static AllocatorPtr create(MemoryPoolPtr memoryPool)
	{
		return AllocatorPtr(new BlockAllocator(std::move(memoryPool)));
	};

	void *allocate() override
	{
		if (nextFreeBlock == nullptr)
		{
			return nullptr;
		}
		void *p = nextFreeBlock;
		nextFreeBlock = (void **)*nextFreeBlock;
		memoryPool_->used += BLOCK_SIZE;

#ifdef DEBUG_ALLOCATOR
		memset(p, 0xFF, BLOCK_SIZE);
		debug_allocate(p);
#endif
		return p;
	}

	void *allocate(size_t, uint8) override
	{
		assert(false && "This method is not supported.");
		return nullptr;
	}

	void free(void *ptr) override
	{
		if (!ptr)
		{
			return;
		}

#ifdef DEBUG_ALLOCATOR
		memset(ptr, 0x00, BLOCK_SIZE);
		debug_free(ptr);
#endif

		assert(memoryPool_->used > 0 && "Memory already free.");

		*((void **)ptr) = nextFreeBlock;
		nextFreeBlock = (void **)ptr;
		memoryPool_->used -= BLOCK_SIZE;
	}
	bool own(void *ptr) const override
	{
		return memoryPool_->own(ptr);
	}

private:
	BlockAllocator(MemoryPoolPtr memoryPool) : memoryPool_(std::move(memoryPool))
	{
		static_assert(BLOCK_SIZE >= sizeof(void *) && "Object size < reference size");
		clear();
	}

	void clear()
	{
		uint8 adjustment = MemoryUtils::AlignAdjustment(memoryPool_->address, ALIGNMENT);
		assert(adjustment < memoryPool_->size && BLOCK_SIZE && "Can't do alignment adjustment. adjustment < maxSize_");

		size_t numObjects = static_cast<size_t>(std::floor((memoryPool_->size - adjustment) / BLOCK_SIZE));
		assert(numObjects > 0 && "Pool allocator can't allocate any object. Not enough memory to one object");

		union {
			void *asVoid;
			uptr asUptr;
		};
		asVoid = memoryPool_->address;
		asUptr += adjustment;

		memoryPool_->used = 0;
		nextFreeBlock = (void **)asVoid;

		void **p = nextFreeBlock;
		for (size_t i = 0; i < numObjects - 1; ++i)
		{
			*p = (void *)((uptr)p + BLOCK_SIZE);
			p = (void **)*p;
		}
		*p = nullptr;
	}
	MemoryPoolPtr memoryPool_;
	void **nextFreeBlock = nullptr;
};

#endif // VENGINE3D_BLOCKALLOCATOR_H
