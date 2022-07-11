//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_TEXTURE_H
#define VENGINE3D_VE_TEXTURE_H

#include "EngineCore.h"

class Texture {
public:
	Texture();
	Texture(const char* path);
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	~Texture();

	void load(const char* path);
	void set(uint32 uniformIndex, uint32 textureIndex);
	void unSet(uint32 textureIndex);
	uint32 getHandle();

protected:
	uint32 width_;
	uint32 height_;
	uint32 channels_;
	uint32 handle_;
};


#endif	//VENGINE3D_VE_TEXTURE_H
