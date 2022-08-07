//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IDMANAGER_H
#define VENGINE3D_IDMANAGER_H

#include <limits>

using TypeId = size_t;
static const TypeId INVALID_TYPE_ID = std::numeric_limits<TypeId>::max();

using ObjectId = size_t;
static const TypeId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();

template <typename T>
struct TypeIdManager
{
	template <typename U>
	static TypeId getId(){
		static const TypeId TYPE_ID = count_++;
		return TYPE_ID;
	}

private:
	static size_t count_;
};

template <typename T>
size_t TypeIdManager<T>::count_ = 0;


#endif // VENGINE3D_IDMANAGER_H
