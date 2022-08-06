//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ENTITY_H
#define VENGINE3D_ENTITY_H

#include "IEntity.h"

template <typename T>
class Entity : public IEntity
{
public:
	virtual const EntityTypeId getEntityTypeId() const override
	{
		return TypeIdManager::getId<T>();
	}
};

#endif // VENGINE3D_ENTITY_H
