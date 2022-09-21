//
// Created by boris on 21.09.2022.
//

#ifndef VENGINE3D_CHUNKALLOCATOR_H
#define VENGINE3D_CHUNKALLOCATOR_H

#include "Core/Memory/Allocator/StackAllocator.h"
#include "Core/Memory/Allocator/SystemAllocator.h"
#include "Core/Memory/Allocator/BlockAllocator.h"
#include <list>
#include <utility>

template <class Allocator>
class ChunkAllocator final : public IAllocator
{
public:
	static AllocatorPtr create(AllocatorPtr externalAllocator, size_t chunkSize, uint8 chunkAlignment)
	{
		return AllocatorPtr(new ChunkAllocator(externalAllocator, chunkSize, chunkAlignment));
	};

	void *allocate(size_t size, uint8 alignment) override
	{
		for (const auto &allocator : chunks_)
		{
			auto ptr = allocator->allocate(size, alignment);
			if (ptr)
			{
				return ptr;
			}
		}
		auto chunkPtr = externalAllocator_->allocate(chunkSize_, chunkAlignment_);
		if (!chunkPtr)
		{
			return nullptr;
		}

		auto mpool = MemoryPool::create(chunkPtr, chunkSize_, externalAllocator_);
		auto allocator = Allocator::create(std::move(mpool));
		auto ptr = allocator->allocate(size, alignment);
		if (!ptr)
		{
			return nullptr;
		}
		chunks_.push_back(allocator);
		return ptr;
	}

	void free(void *ptr) override
	{
		auto allocator = getAllocator(ptr);
		if (!allocator)
		{
			return;
		}
		allocator->free(ptr);
	}

	bool own(void *ptr) const override
	{
		for (const auto &allocator : chunks_)
		{
			if (allocator->own(ptr))
			{
				return true;
			}
		}
		return false;
	}

//private:
	explicit ChunkAllocator(AllocatorPtr externalAllocator, size_t chunkSize, uint8 chunkAlignment)
		: externalAllocator_(std::move(externalAllocator)), chunkSize_(chunkSize), chunkAlignment_(chunkAlignment)
	{
	}

	AllocatorPtr getAllocator(void *ptr)
	{
		for (const auto &allocator : chunks_)
		{
			if (allocator->own(ptr))
			{
				return allocator;
			}
		}
		assert(false && "MemoryManager has not this ptr");
		return {};
	}

	const size_t chunkSize_;
	const size_t chunkAlignment_;

	AllocatorPtr externalAllocator_;
	std::list<AllocatorPtr> chunks_;
};

#endif // VENGINE3D_CHUNKALLOCATOR_H
