//
// Created by boris on 7/21/2022.
//
#include "StackAllocator.h"
#include "EngineLibs.h"
#include "ECS/Memory/MemoryUtils.h"

StackAllocator::StackAllocator(const std::shared_ptr<MemoryPool> &memoryPool) : memoryPool_(memoryPool)
{
	issuedMemory_.reserve(10);
	freedMemory_.reserve(10);
}

void *StackAllocator::allocate(size_t size, uint8 alignment)
{
	auto ptr = allocate_memory(size, alignment);
	if (ptr)
	{
		issuedMemory_.push_back(ptr);
	}
	return ptr;
}

void StackAllocator::free(void *ptr)
{
	if (!ptr)
	{
		return;
	}
	assert(std::find(issuedMemory_.begin(), issuedMemory_.end(), ptr) != issuedMemory_.end());
	if (ptr == issuedMemory_.back())
	{
		deallocate_memory(ptr);
		issuedMemory_.pop_back();

		bool check = true;
		while (check)
		{
			check = false;
			for (auto &freedPtr : freedMemory_)
			{
				if (freedPtr == issuedMemory_.back())
				{
					deallocate_memory(freedPtr);
					issuedMemory_.pop_back();
					std::swap(freedPtr, freedMemory_.back());
					freedMemory_.pop_back();
					check = true;
					break;
				}
			}
		}
	}
	else
	{
		freedMemory_.push_back(ptr);
	}
}

void *StackAllocator::allocate_memory(size_t size, uint8 alignment)
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

	auto adjustment = MemoryUtils::AlignAdjustment(asVoid, alignment, sizeof(MetaInfo));
	if (memoryPool_->used + size + adjustment > memoryPool_->size)
	{
		return nullptr;
	}

	asUptr += adjustment;
	MetaInfo *meta = (MetaInfo *)(asUptr - sizeof(MetaInfo));
	meta->adjustment = adjustment;

	memoryPool_->used += size + adjustment;
#ifdef DEBUG_ALLOCATOR
	memset(asVoid, 0xFF, size);
	debug_allocate(asVoid);
#endif
	return asVoid;
}

void StackAllocator::deallocate_memory(void *ptr)
{
	assert(memoryPool_->used > 0 && "Memory already free.");
	if (memoryPool_->used == 0)
	{
		return;
	}

	union {
		void *asVoid;
		uptr asUptr;
	};
	asVoid = ptr;
	MetaInfo *meta = (MetaInfo *)(asUptr - sizeof(MetaInfo));

	asUptr -= meta->adjustment;

	auto freedMemorySize = (memoryPool_->addressUptr + memoryPool_->used) - asUptr;
	memoryPool_->used -= freedMemorySize;

#ifdef DEBUG_ALLOCATOR
	memset(asVoid, 0x00, freedMemorySize);
	debug_free(ptr);
#endif
}
bool StackAllocator::own(void *ptr) const
{
	return memoryPool_->own(ptr);
}
