//
// Created by boris on 8/7/2022.
//
#ifndef VENGINE3D_TYPEIDMANAGE_H
#define VENGINE3D_TYPEIDMANAGE_H

#include "EnginePlatform.h"

template <typename T>
struct TypeIdManager
{
	template <typename U>
	static idtype getId(){
		static const idtype TYPE_ID = count_++;
		return TYPE_ID;
	}

	static idtype getCount(){
		return count_;
	}
private:
	static idtype count_;
};

template <typename T>
size_t TypeIdManager<T>::count_ = 0;

#endif // VENGINE3D_TYPEIDMANAGE_H
