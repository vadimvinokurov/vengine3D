//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ENTITYMANAGER_H
#define VENGINE3D_ENTITYMANAGER_H

#include "ECS/Memory/VObjectContainer.h"
#include "ECS/Objects/Entity.h"
#include <unordered_map>
static constexpr auto ENITY_CHUNK_SIZE = 512;
class EntityManager
{
private:
	struct IEntityContainer
	{
		virtual void destroyEntity(IEntity *entity) = 0;
	};
	template <class T>
	struct EntityContainer : public VObjectContainer<T, ENITY_CHUNK_SIZE>, public IEntityContainer
	{
		virtual void destroyEntity(IEntity *entity) override
		{
			this->destroyObject(entity);
		};
	};

public:
	template <typename T, typename... Args>
	EntityId createEntity(Args... args)
	{
		EntityContainer<T> *container = getContainer<T>();
		auto entity = container->createObject();
		auto entityId = entityHandleTable_.AcquiredHandle(entity);
		entity->id_ = entityId;
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
			IEntity *entity = entityHandleTable_[entityId];
			auto it = entityContainers_.find(entity->getEntityTypeId());
			if (it != entityContainers_.end())
			{
				it->second->destroyEntity(entity);
			}
			entityHandleTable_.ReleaseHandle(entityId);
		}
		pendingDestroyedEntities_.clear();
	}

private:
	template <typename T>
	EntityContainer<T> *getContainer()
	{
		auto ETID = T::getTypeId();
		auto it = entityContainers_.find(ETID);
		if (it == entityContainers_.end())
		{
			auto ec = std::make_shared<EntityContainer<T>>();
			entityContainers_[ETID] = ec;
			return ec.get();
		}
		else
		{
			return std::static_pointer_cast<EntityContainer<T>>(it->second).get();
		}
	}

	ECS::HandleTable<IEntity, EntityId> entityHandleTable_;
	std::unordered_map<EntityTypeId, std::shared_ptr<IEntityContainer>> entityContainers_;
	std::vector<EntityId> pendingDestroyedEntities_;
};

#endif // VENGINE3D_ENTITYMANAGER_H
