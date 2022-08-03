//
// Created by boris on 7/22/2022.
//
#include "BlockAllocator.h"
#include "EngineLibs.h"
#include "MemoryUtils.h"

BlockAllocator::BlockAllocator(const std::shared_ptr<MemoryPool> &memoryPool, size_t objectSize, uint8 objectAlignment)
	: memoryPool_(memoryPool), OBJECT_SIZE(objectSize), OBJECT_ALIGNMENT(objectAlignment)
{
	assert(objectSize > 0 && "Object size = 0");
	clear();
}

void *BlockAllocator::allocate(size_t, uint8)
{
	if (nextFreeBlock == nullptr)
	{
		return nullptr;
	}
	void *p = nextFreeBlock;
	nextFreeBlock = (void **)*nextFreeBlock;
	memoryPool_->used += OBJECT_SIZE;

	return p;
}

void BlockAllocator::free(void *ptr)
{
	if (!ptr)
	{
		return;
	}

	assert(memoryPool_->used > 0 && "Memory already free.");

	*((void **)ptr) = nextFreeBlock;
	nextFreeBlock = (void **)ptr;
	memoryPool_->used -= OBJECT_SIZE;
}

void BlockAllocator::clear()
{
	uint8 adjustment = MemoryUtils::AlignAdjustment(memoryPool_->address, OBJECT_ALIGNMENT);
	assert(adjustment < memoryPool_->size && OBJECT_SIZE && "Can't do alignment adjustment. adjustment < maxSize_");

	size_t numObjects = static_cast<size_t>(std::floor((memoryPool_->size - adjustment) / OBJECT_SIZE));
	assert(numObjects > 0 && "Pool allocator can't allocate any object. Not enough memory to one object");

	union {
		void *asVoid;
		uptr *asUptr;
	};
	asVoid = memoryPool_->address;
	asUptr += adjustment;

	memoryPool_->used = 0;
	nextFreeBlock = (void **)asVoid;

	void **p = nextFreeBlock;
	for (size_t i = 0; i < numObjects - 1; ++i)
	{
		*p = (void *)((uptr)p + OBJECT_SIZE);
		p = (void **)*p;
	}
	*p = nullptr;
}
bool BlockAllocator::own(void *ptr) const
{
	return memoryPool_->own(ptr);
}
