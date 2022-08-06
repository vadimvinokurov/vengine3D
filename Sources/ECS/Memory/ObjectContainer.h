//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_OBJECTMANAGER_H
#define VENGINE3D_OBJECTMANAGER_H

#include "ECS/Objects/VObject.h"
#include "GlobalMemoryManager.h"
#include "ECS/Memory/Allocator/BlockAllocator.h"
#include <list>

template <typename T, size_t MAX_CHUNK_SIZE>
class ObjectContainer
{
private:
	static constexpr auto CHUNK_MEMORY_SIZE = MAX_CHUNK_SIZE * (sizeof(T) + alignof(T));

	using Allocator = BlockAllocator<sizeof(T), alignof(T)>;
	using ObjectList = std::vector<T *>;

	struct Chunk
	{
		explicit Chunk(const std::shared_ptr<Allocator> &alloc) : allocator(alloc)
		{
			objects.reserve(MAX_CHUNK_SIZE);
		}
		Chunk(const Chunk &other) = delete;
		Chunk *operator=(const Chunk &other) = delete;
		Chunk(Chunk &&other) = delete;
		Chunk *operator=(Chunk &&other) = delete;

		std::shared_ptr<Allocator> allocator;
		ObjectList objects;
	};
	using MemoryChunks = std::list<Chunk>;

public:
	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		iterator(typename MemoryChunks::iterator begin, typename MemoryChunks::iterator end)
			: currentChunk_(begin), end_(end)
		{
			static_assert(MAX_CHUNK_SIZE > 0);
			if (begin != end)
			{
				currentObject_ = currentChunk_->objects.begin();
			}
			else
			{
				currentObject_ = std::prev(end_)->objects.end();
			}
		}

		iterator &operator++()
		{
			++currentObject_;
			if (currentObject_ == currentChunk_->objects.end())
			{
				++currentChunk_;
				if (currentChunk_ != end_)
				{
					currentObject_ = currentChunk_->objects.begin();
				}
			}
			return *this;
		}

		T &operator*() const
		{
			return *(*currentObject_);
		}

		T *operator->() const
		{
			return *currentObject_;
		}

		bool operator==(const iterator &other) const
		{
			return (this->currentChunk_ == other.currentChunk_) && (this->currentObject_ == other.currentObject_);
		}
		bool operator!=(const iterator &other) const
		{
			return (this->currentChunk_ != other.currentChunk_) || (this->currentObject_ != other.currentObject_);
		}

	private:
		typename MemoryChunks::iterator currentChunk_;
		typename MemoryChunks::iterator end_;
		typename ObjectList::iterator currentObject_;
	};

	ObjectContainer() = default;

	template <typename... Args>
	T *createObject(Args &&...args)
	{
		for (auto &chunk : chunks_)
		{
			auto ptr = chunk.allocator->allocate();
			if (!ptr)
			{
				continue;
			}
			chunk.objects.push_back((T *)ptr);
			new (ptr) T(std::forward<Args>(args)...);
			return (T *)ptr;
		}

		auto mpool = GlobalMemoryManager::allocateMemoryPool(CHUNK_MEMORY_SIZE);
		chunks_.emplace_back(std::make_shared<Allocator>(mpool));
		auto &chunk = chunks_.back();
		auto ptr = chunk.allocator->allocate();

		assert(ptr != nullptr && "Object manager can't allocate memory. Out of memory!");
		chunk.objects.push_back((T *)ptr);
		new (ptr) T(std::forward<Args>(args)...);
		return (T *)ptr;
	}

	void destroyObject(VObject *ptr)
	{
		for (auto &chunk : chunks_)
		{
			if (chunk.allocator->own((void *)ptr))
			{
				ptr->~VObject();
				auto it = std::remove(chunk.objects.begin(), chunk.objects.end(), (T *)ptr);
				chunk.objects.erase(it);
				chunk.allocator->free((void *)ptr);
				return;
			}
		}
	}

	iterator begin()
	{
		return iterator(chunks_.begin(), chunks_.end());
	}

	iterator end()
	{
		return iterator(chunks_.end(), chunks_.end());
	}

	~ObjectContainer()
	{
		for (auto &chunk : chunks_)
		{
			for (auto ptr : chunk.objects)
			{
				ptr->~T();
				chunk.allocator->free((void *)ptr);
			}
			chunk.objects.clear();
		}
	}

private:
	std::list<Chunk> chunks_;
};
#endif // VENGINE3D_OBJECTMANAGER_H
