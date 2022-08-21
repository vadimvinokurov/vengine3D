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
		textures.emplace_back(Texture("E:\\Work\\vengine3D\\Content\\Mesh\\HeroTPP_BaseColor.png"), "baseColorTexture");
		vertexShader.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
	}
	std::vector<std::pair<Texture, std::string>> textures;
	std::vector<ShaderSource> vertexShader;
};

#endif // VENGINE3D_MATERIAL_H
