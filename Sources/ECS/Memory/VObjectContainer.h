//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_OBJECTMANAGER_H
#define VENGINE3D_OBJECTMANAGER_H

#include "ECS/Objects/VObject.h"
#include "GlobalMemoryManager.h"
#include "ECS/Memory/Allocator/BlockAllocator.h"
#include <list>

class IVObjectContainer
{
public:
	IVObjectContainer() = default;
	virtual ~IVObjectContainer() = default;
	virtual void destroyObject(VObject *ptr) = 0;
};

template <typename T, size_t MAX_CHUNK_SIZE>
class VObjectContainer : public IVObjectContainer
{
private:
	static constexpr auto CHUNK_MEMORY_SIZE = MAX_CHUNK_SIZE * sizeof(T) + alignof(T);

	using Allocator = BlockAllocator<sizeof(T), alignof(T)>;
	using ObjectList = std::vector<T *>;

	struct Chunk
	{
		explicit Chunk(AllocatorPtr alloc) : allocator(alloc)
		{
			objects.reserve(MAX_CHUNK_SIZE);
		}
		Chunk(const Chunk &other) = delete;
		Chunk *operator=(const Chunk &other) = delete;
		Chunk(Chunk &&other) = delete;
		Chunk *operator=(Chunk &&other) = delete;

		AllocatorPtr allocator;
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

	VObjectContainer() = default;

	template <typename... Args>
	T *createObject(Args &&...args)
	{
		void *ptr = allocate();
		if (!ptr)
		{
			return nullptr;
		}
		try
		{
			new (ptr) T(std::forward<Args>(args)...);
			return (T *)ptr;
		}
		catch (std::exception &e)
		{
			free(ptr);
			spdlog::error("VObject constructor exception: {}", e.what());
			assert(false && "Exception in VObject constructor");
			return nullptr;
		}
	}

	void destroyObject(VObject *ptr) override
	{
		if (!ptr)
		{
			return;
		}
		ptr->~VObject();
		free(ptr);
	}

	iterator begin()
	{
		return iterator(chunks_.begin(), chunks_.end());
	}

	iterator end()
	{
		return iterator(chunks_.end(), chunks_.end());
	}

	~VObjectContainer()
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
	void *allocate()
	{
		for (auto &chunk : chunks_)
		{
			auto ptr = chunk.allocator->allocate();
			if (ptr)
			{
				chunk.objects.push_back((T *)ptr);
				return ptr;
			}
			assert(chunk.objects.size() == MAX_CHUNK_SIZE && "Number of object in chunk != MAX_CHUNK_SIZE");
		}

		auto mpool = GlobalMemoryManager::allocateMemoryPool(CHUNK_MEMORY_SIZE);
		chunks_.emplace_back(Allocator::create(std::move(mpool)));
		auto &chunk = chunks_.back();
		auto ptr = chunk.allocator->allocate();
		if (ptr)
		{
			chunk.objects.push_back((T *)ptr);
			return ptr;
		}

		assert(false && "Object manager can't allocate memory. Out of memory!");
		return nullptr;
	}

	void free(void *ptr)
	{
		for (auto &chunk : chunks_)
		{
			if (chunk.allocator->own((void *)ptr))
			{

				auto it = std::remove(chunk.objects.begin(), chunk.objects.end(), (T *)ptr);
				chunk.objects.erase(it);
				chunk.allocator->free((void *)ptr);
				return;
			}
		}
	}

	std::list<Chunk> chunks_;
};
#endif // VENGINE3D_OBJECTMANAGER_H
