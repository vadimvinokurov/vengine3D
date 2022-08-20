//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_MATERIAL_H
#define VENGINE3D_MATERIAL_H

#include "Render/Texture.h"
#include "Render/Shader.h"

struct Material
{
	Material()
	{
		textures.emplace_back("../Content/Texture/test.png");
		vertexShader.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
	}
	std::vector<Texture> textures;
	std::vector<ShaderSource> vertexShader;
};

#endif // VENGINE3D_MATERIAL_H
