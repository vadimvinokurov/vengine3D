//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_OBJECTMANAGER_H
#define VENGINE3D_OBJECTMANAGER_H

#include "GlobalMemoryManager.h"
#include "Allocator/BlockAllocator.h"
#include <unordered_set>

template <typename T, size_t MAX_CHUNK_SIZE>
class ObjectManager
{
private:
	using Allocator = BlockAllocator<sizeof(T), alignof(T)>;
	static constexpr auto CHUNK_MEMORY_SIZE = MAX_CHUNK_SIZE * (sizeof(T) + alignof(T));
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
		std::unordered_set<T *> objects;
	};

public:
	ObjectManager() = default;

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
			chunk.objects.emplace((T *)ptr);
			new (ptr) T(std::forward<Args>(args)...);
			return (T *)ptr;
		}

		auto mpool = GlobalMemoryManager::allocateMemoryPool(CHUNK_MEMORY_SIZE);
		chunks_.emplace_back(std::make_shared<Allocator>(mpool));
		auto &chunk = chunks_.back();
		auto ptr = chunk.allocator->allocate();

		assert(ptr != nullptr && "Object manager can't allocate memory. Out of memory!");
		chunk.objects.emplace((T *)ptr);
		new (ptr) T(std::forward<Args>(args)...);
		return (T *)ptr;
	}

	void destroyObject(T *ptr)
	{
		for (auto &chunk : chunks_)
		{
			if (chunk.allocator->own((void *)ptr))
			{
				ptr->~T();
				chunk.objects.erase(ptr);
				chunk.allocator->free((void *)ptr);
				return;
			}
		}
	}

	~ObjectManager() {
		for (auto &chunk : chunks_)
		{
			for(auto ptr: chunk.objects) {
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
