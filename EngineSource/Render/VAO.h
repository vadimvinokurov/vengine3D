//
// Created by boris on 8/31/2022.
//

#ifndef VENGINE3D_VAO_H
#define VENGINE3D_VAO_H
#include "EnginePlatform.h"
class VAO
{
public:
	VAO();
	~VAO();
	void bind() const;
	void unbind() const;

	VAO(const VAO &other) = delete;
	VAO &operator=(const VAO &other) = delete;
	VAO(VAO &&other);
	VAO &operator=(VAO &&other);

private:
	uint32 handle_;
};

#endif // VENGINE3D_VAO_H
