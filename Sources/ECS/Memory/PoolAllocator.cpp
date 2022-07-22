//
// Created by boris on 7/22/2022.
//
#include "PoolAllocator.h"
#include "EngineLibs.h"
#include "MemoryUtils.h"

PoolAllocator::PoolAllocator(void *memory, size_t size, size_t objectSize, uint8 objectAlignment)
	: IAllocator(memory, size), OBJECT_SIZE(objectSize), OBJECT_ALIGNMENT(objectAlignment)
{
	assert(objectSize > 0 && "Object size = 0");
	clear();
}

void *PoolAllocator::allocate(size_t size, uint8 alignment)
{
	assert(size > 0 && "allocate called with memSize = 0.");
	assert(size == OBJECT_SIZE && alignment == OBJECT_ALIGNMENT);

	if (nextFreeBlock == nullptr)
	{
		return nullptr;
	}
	void *p = nextFreeBlock;
	nextFreeBlock = (void **)*nextFreeBlock;
	usedMemory_ += OBJECT_SIZE;

	return p;
}

void PoolAllocator::deallocate(void *ptr)
{
	if (!ptr)
	{
		return;
	}

	assert(usedMemory_ > 0 && "Memory already free.");

	*((void **)ptr) = nextFreeBlock;
	nextFreeBlock = (void **)ptr;
	usedMemory_ -= OBJECT_SIZE;
}

void PoolAllocator::clear()
{
	uint8 adjustment = MemoryUtils::AlignAdjustment(baseAddress_, OBJECT_ALIGNMENT);
	assert(adjustment < maxSize_ && OBJECT_SIZE && "Can't do alignment adjustment. adjustment < maxSize_");

	size_t numObjects = static_cast<size_t>(std::floor((maxSize_ - adjustment) / OBJECT_SIZE));
	assert(numObjects > 0 && "Pool allocator can't allocate any object. Not enough memory to one object");

	union {
		void *asVoid;
		uptr *asUptr;
	};
	asVoid = baseAddress_;
	asUptr += adjustment;

	usedMemory_ = 0;
	nextFreeBlock = (void **)asVoid;

	void **p = nextFreeBlock;
	for (size_t i = 0; i < numObjects - 1; ++i)
	{
		*p = (void *)((uptr)p + OBJECT_SIZE);
		p = (void **)*p;
	}
	*p = nullptr;
}
