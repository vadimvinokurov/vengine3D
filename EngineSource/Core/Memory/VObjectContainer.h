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
	using MemoryChunks = ChunkAllocator<Allocator>;
	using ObjectList = std::vector<T *>;

public:
	using iterator = typename ObjectList::iterator;
	VObjectContainer() : chunks_(GlobalMemoryManager::get(), CHUNK_MEMORY_SIZE, 1){};

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
		return objects.begin();
	}

	iterator end()
	{
		return objects.end();
	}

	~VObjectContainer() override
	{
		for (auto ptr : objects)
		{
			ptr->~T();
			chunks_.free((void *)ptr);
		}
	}

private:
	void *allocate()
	{
		auto ptr = chunks_.allocate(sizeof(T), alignof(T));
		if (!ptr)
		{
			return nullptr;
		}

		objects.push_back((T *)ptr);
	}

	void free(void *ptr)
	{
		auto it = std::remove(objects.begin(), objects.end(), (T *)ptr);
		objects.erase(it);
		chunks_.free(ptr);
	}

	MemoryChunks chunks_;
	ObjectList objects;

#ifdef ECS_DEBUG
	std::set<VObject *> ptrs;
#endif
};
#endif // VENGINE3D_OBJECTMANAGER_H
