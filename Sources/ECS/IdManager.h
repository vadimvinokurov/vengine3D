//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_TYPEIDMANAGER_H
#define VENGINE3D_TYPEIDMANAGER_H

#include <limits>

using TypeId = size_t;
static const TypeId INVALID_TYPE_ID = std::numeric_limits<TypeId>::max();

using ObjectId = size_t;
static const TypeId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();

struct TypeIdManager
{
	template <typename T>
	static TypeId get(){
		static const TypeId TYPE_ID = count_++;
		return TYPE_ID;
	}

private:
	static size_t count_;
};


#endif // VENGINE3D_TYPEIDMANAGER_H
