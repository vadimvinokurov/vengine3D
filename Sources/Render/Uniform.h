//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_UNIFORM_H
#define VENGINE3D_VE_UNIFORM_H

#include <vector>
#include "EnginePlatform.h"

namespace Render
{
	template <typename T>
	struct Uniform
	{
		static void set(unsigned int slot, const T &value);
		static void set(unsigned int slot, const std::vector<T> &arr);
		static void set(unsigned int slot, const T *arr, uint32 arraySize);
	};
} // namespace Render

#endif // VENGINE3D_VE_UNIFORM_H
