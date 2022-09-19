//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_OBJECTMANAGER_H
#define VENGINE3D_OBJECTMANAGER_H

#include "Core/Objects/VObject.h"
#include "Core/Memory/GlobalMemoryManager.h"
#include "Core/Memory/Allocator/BlockAllocator.h"
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
	static_assert(MAX_CHUNK_SIZE > 0);

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
			: currentChunk_(begin), endChunk_(end)
		{
			if (begin != end)
			{
				for (; currentChunk_ != endChunk_; ++currentChunk_)
				{
					currentObject_ = currentChunk_->objects.begin();
					if (currentObject_ != currentChunk_->objects.end())
					{
						return;
					}
				}
			}
			else
			{
				currentObject_ = std::prev(endChunk_)->objects.end();
			}
		}

		iterator &operator++()
		{
			++currentObject_;

			if (currentObject_ == currentChunk_->objects.end())
			{
				++currentChunk_;
				for (; currentChunk_ != endChunk_; ++currentChunk_)
				{
					currentObject_ = currentChunk_->objects.begin();
					if (currentObject_ != currentChunk_->objects.end())
					{
						return *this;
					}
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
		typename MemoryChunks::iterator endChunk_;
		typename ObjectList::iterator currentObject_;
	};

	VObjectContainer()
	{
		auto mpool = GlobalMemoryManager::allocateMemoryPool(CHUNK_MEMORY_SIZE);
		chunks_.emplace_back(Allocator::create(std::move(mpool)));
	}

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
#ifdef ECS_DEBUG
			ptrs.insert((T *)ptr);
#endif
			return (T *)ptr;
		}
		catch (std::exception &e)
		{
			free(ptr);
			spdlog::error("VObject constructor exception: {}", e.what());
			assert(false);
			return nullptr;
		}
		catch (...)
		{
			free(ptr);
			spdlog::error("VObject constructor exception: unknown");
			assert(false);
			return nullptr;
		}
	}

	void destroyObject(VObject *ptr) override
	{
		if (!ptr)
		{
			return;
		}
#ifdef ECS_DEBUG
		assert(ptrs.find(ptr) != ptrs.end() && "Try to free incorrect VObject ptr.");
		ptrs.erase(ptr);
#endif
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

		assert(false && "VObject manager can't allocate memory. Out of memory!");
		return nullptr;
	}

	void free(void *ptr)
	{
		for (auto &chunk : chunks_)
		{
			if (chunk.allocator->own(ptr))
			{

				auto it = std::remove(chunk.objects.begin(), chunk.objects.end(), (T *)ptr);
				chunk.objects.erase(it);
				chunk.allocator->free(ptr);
				return;
			}
		}
	}

	std::list<Chunk> chunks_;

#ifdef ECS_DEBUG
	std::set<VObject *> ptrs;
#endif
};
#endif // VENGINE3D_OBJECTMANAGER_H
