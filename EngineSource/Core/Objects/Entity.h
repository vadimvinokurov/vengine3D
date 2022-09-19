//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IENTITY_H
#define VENGINE3D_IENTITY_H

#include "VObject.h"
#include "Core/Utils/IdManagers.h"
#include "Core/Managers/ComponentManager.h"
#include "Core/Utils/TypeTraits.h"

#define GENERATE_ENTITY_BODY(BasedClass)                                                                               \
public:                                                                                                                \
	using Super = BasedClass;                                                                                          \
	virtual EntityTypeId getEntityTypeId() const                                                                       \
	{                                                                                                                  \
		return TypeIdManager<Entity>::getId<std::remove_cv_t<std::remove_pointer_t<decltype(this)>>>();                \
	}                                                                                                                  \
	virtual EntityTypeId getParentEntityTypeId() const                                                                 \
	{                                                                                                                  \
		return TypeIdManager<Entity>::getId<Super>();                                                                  \
	}                                                                                                                  \
	static EntityTypeId getTypeId()                                                                                    \
	{                                                                                                                  \
		return TypeIdManager<Entity>::getId<get_class_type_by_method_t<decltype(&getEntityTypeId)>>();                 \
	}                                                                                                                  \
                                                                                                                       \
private:
class Entity : public VObject
{
	friend class EntityManager;
	GENERATE_ENTITY_BODY(VObject)

public:
	Entity() = default;
	virtual ~Entity() = default;

	EntityId getEntityId() const
	{
		return id_;
	}

	template <typename T, typename... Args>
	T *addComponent(Args &&...args)
	{
		return componentManage_->addComponent<T>(id_, std::forward<Args>(args)...);
	}

	template <typename T>
	T *getComponent()
	{
		return componentManage_->getComponent<T>(id_);
	}

	template <typename T>
	void removeComponent()
	{
		componentManage_->removeComponent<T>(id_);
	}

	void setActive(bool active)
	{
		if (active_ == active)
			return;

		if (active == false)
		{
			this->onDisable();
		}
		else
		{
			this->onEnable();
		}

		active_ = active;
	}

	bool isActive()
	{
		return active_;
	}

	bool operator==(const Entity &other) const
	{
		return id_ == other.id_;
	}
	bool operator!=(const Entity &other) const
	{
		return id_ != other.id_;
	}
	bool operator==(const Entity *other) const
	{
		return id_ == other->id_;
	}
	bool operator!=(const Entity *other) const
	{
		return id_ != other->id_;
	}

	virtual void onDisable()
	{
	}

	virtual void onEnable()
	{
	}

	virtual void onCreate()
	{
	}

	virtual void onQuite()
	{
	}

	virtual void tick(float dt)
	{
	}

protected:
	EntityId id_ = INVALID_ID;
	bool active_ = true;
	ComponentManager *componentManage_ = nullptr;
};

#endif // VENGINE3D_IENTITY_H
