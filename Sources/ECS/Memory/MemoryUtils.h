//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_MEMORYUTILS_H
#define VENGINE3D_MEMORYUTILS_H

#include "EnginePlatform.h"

namespace MemoryUtils
{
	inline uint8 AlignAdjustment(void *address, uint8 alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		return adjustment == alignment ? 0 : adjustment;
	}

	inline uint8 AlignAdjustment(void *address, uint8 alignment, uint8 extra)
	{
		uint8 adjustment = AlignAdjustment(address, alignment);
		uint8 neededSpace = extra;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}
} // namespace MemoryUtils

#endif // VENGINE3D_MEMORYUTILS_H
