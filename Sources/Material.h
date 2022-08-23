//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_MATERIAL_H
#define VENGINE3D_MATERIAL_H

#include "Render/Texture.h"
#include "Render/Shader.h"
#include <memory>
#include <vector>
#include <iostream>

struct Material
{
	Material(std::string s)
	{
		textures.emplace_back(Texture(s.c_str()), "baseColorTexture");
		std::vector<ShaderSource> shaderSource;
		shaderSource.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
		shaderSource.emplace_back("../Content/Shaders/default.vert", ShaderType::Vertex);
		shader = std::make_shared<Shader>(shaderSource);
	}
	Material(const Material &other) = delete;
	Material(Material &&other) = delete;
	Material &operator=(const Material &other) = delete;
	Material &operator=(Material &&other) = delete;

	std::vector<std::pair<Texture, std::string>> textures;
	std::shared_ptr<Shader> shader;
};

#endif // VENGINE3D_MATERIAL_H
