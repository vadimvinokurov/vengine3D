//
// Created by boris on 7/21/2022.
//
#include "StackAllocator.h"
#include "EngineLibs.h"
#include "MemoryUtils.h"

StackAllocator::StackAllocator(void *memory, size_t size) : IAllocator(memory, size)
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

void StackAllocator::deallocate(void *ptr)
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

void StackAllocator::clear()
{
	usedMemory_ = 0;
}

void *StackAllocator::allocate_memory(size_t size, uint8 alignment)
{
	assert(size > 0 && "allocate called with memSize = 0.");

	union {
		void *asVoid;
		uptr asUptr;
	};
	asVoid = baseAddress_;
	asUptr += usedMemory_;

	auto adjustment = MemoryUtils::AlignAdjustment(asVoid, alignment, sizeof(MetaInfo));
	if (usedMemory_ + size + adjustment > maxSize_)
	{
		return nullptr;
	}

	asUptr += adjustment;
	MetaInfo *meta = (MetaInfo *)(asUptr - sizeof(MetaInfo));
	meta->adjustment = adjustment;

	usedMemory_ += size + adjustment;
	//memset(asVoid, 0xFF, size);
	return asVoid;
}

void StackAllocator::deallocate_memory(void *ptr)
{
	assert(usedMemory_ > 0 && "Memory already free.");

	union {
		void *asVoid;
		uptr asUptr;
	};
	asVoid = ptr;
	MetaInfo *meta = (MetaInfo *)(asUptr - sizeof(MetaInfo));

	asUptr -= meta->adjustment;

	auto freedMemorySize = ((uptr)baseAddress_ + usedMemory_) - asUptr;
	usedMemory_ -= freedMemorySize;

	//memset(asVoid, 0x00, freedMemorySize);
}
