//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ENTITYMANAGER_H
#define VENGINE3D_ENTITYMANAGER_H

#include "ECS/Memory/VObjectContainer.h"
#include "ECS/Objects/Entity.h"
#include <unordered_map>

static constexpr auto ENTITY_CHUNK_SIZE = 512;
class EntityManager
{
private:
	template <class T>
	using EntityContainer = VObjectContainer<T, ENTITY_CHUNK_SIZE>;
	using IEntityContainer = IVObjectContainer;
	using ContainersMap = std::unordered_map<EntityTypeId, std::unique_ptr<IEntityContainer>>;

public:
	EntityManager(ComponentManager *componentManage) : componentManage_(componentManage)
	{
	}
	template <typename T, typename... Args>
	EntityId createEntity(Args... args)
	{
		T *entity = getEntityContainer<T>()->createObject(std::forward<Args>(args)...);
		EntityId entityId = aqcuireEntityId(entity);
		entity->entityId_ = entityId;
		entity->componentManage_ = componentManage_;
		return entityId;
	}

	void destroyEntity(EntityId entityId)
	{
		pendingDestroyedEntities_.push_back(entityId);
	}

	void removeDestroyedEntities()
	{
		for (const auto &entityId : pendingDestroyedEntities_)
		{
			IEntity *entity = getEntityById(entityId);
			auto it = entityContainers_.find(entity->getEntityTypeId());
			if (it != entityContainers_.end())
			{
				it->second->destroyObject(entity);
			}
			releaseEntityId(entityId);
		}
		pendingDestroyedEntities_.clear();
	}

	IEntity *getEntityById(EntityId entityId)
	{
		return entityIdManager_[entityId];
	}

private:
	template <typename T>
	EntityContainer<T> *getEntityContainer()
	{
		auto entityTypeId = T::getTypeId();
		auto it = entityContainers_.find(entityTypeId);
		if (it == entityContainers_.end())
		{
			auto container = std::make_unique<EntityContainer<T>>();
			auto ptr = container.get();
			entityContainers_[entityTypeId] = std::move(container);
			return ptr;
		}
		else
		{
			return static_cast<EntityContainer<T> *>(it->second.get());
		}
	}

	EntityId aqcuireEntityId(IEntity *entity)
	{
		return entityIdManager_.acquiredId(entity);
	}

	void releaseEntityId(EntityId id)
	{
		entityIdManager_.releaseId(id);
	}

	ObjectIdManager<IEntity> entityIdManager_;
	ContainersMap entityContainers_;
	std::vector<EntityId> pendingDestroyedEntities_;
	ComponentManager *componentManage_;
};

#endif // VENGINE3D_ENTITYMANAGER_H
