//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_INDEX_BUFFER_H
#define VENGINE3D_VE_INDEX_BUFFER_H

#include <vector>
#include "EnginePlatform.h"

namespace Render
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const IndexBuffer &other) = delete;
		IndexBuffer &operator=(const IndexBuffer &other) = delete;

		IndexBuffer(IndexBuffer &&other);
		IndexBuffer &operator=(IndexBuffer &&other);

		IndexBuffer();
		~IndexBuffer();

		void set(const uint32 *array, uint32 arrayLength);
		void set(const std::vector<uint32> &input);

		void attachToShader() const;

		uint32 count() const;
		uint32 getHandle() const;

	protected:
		uint32 handle_;
		uint32 count_;
	};
} // namespace Render

#endif // VENGINE3D_VE_INDEX_BUFFER_H
