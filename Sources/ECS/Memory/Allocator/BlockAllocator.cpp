//
// Created by boris on 7/22/2022.
//
#include "BlockAllocator.h"
#include "EngineLibs.h"
#include "ECS/Memory/MemoryUtils.h"

BlockAllocator::BlockAllocator(const std::shared_ptr<MemoryPool> &memoryPool, size_t blockSize, uint8 alignment)
	: memoryPool_(memoryPool), BLOCK_SIZE(blockSize), ALIGNMENT(alignment)
{
	assert(blockSize > sizeof(void *) && "Object size < reference size");
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
	memoryPool_->used += BLOCK_SIZE;

#ifdef DEBUG_ALLOCATOR
	memset(p, 0xFF, BLOCK_SIZE);
	debug_allocate(p);
#endif
	return p;
}

void BlockAllocator::free(void *ptr)
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

void BlockAllocator::clear()
{
	uint8 adjustment = MemoryUtils::AlignAdjustment(memoryPool_->address, ALIGNMENT);
	assert(adjustment < memoryPool_->size && BLOCK_SIZE && "Can't do alignment adjustment. adjustment < maxSize_");

	size_t numObjects = static_cast<size_t>(std::floor((memoryPool_->size - adjustment) / BLOCK_SIZE));
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
		*p = (void *)((uptr)p + BLOCK_SIZE);
		p = (void **)*p;
	}
	*p = nullptr;
}
bool BlockAllocator::own(void *ptr) const
{
	return memoryPool_->own(ptr);
}
