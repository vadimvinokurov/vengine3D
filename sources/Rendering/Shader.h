//
// Created by boris on 2/6/2022.
//

#ifndef VENGINE3D_VE_SHADER_H
#define VENGINE3D_VE_SHADER_H

#include "EngineCore.h"

enum class ShaderType : uint8_t {
	Vertex,
	Fragment
};

struct ShaderSource {
	std::string sourceFile;
	ShaderType type;
};

class Shader {
public:
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader();
	~Shader();
	Shader(const std::vector<ShaderSource>& shaders);
	void load(const std::vector<ShaderSource>& shaders);

	void bind();
	void unBind();

	uint32 getAttribute(const std::string& name);
	uint32 getUniform(const std::string& name);
	uint32 getHandle();

private:
	std::string readFile(const std::string& path);
	uint32 compileShade(const std::string& shaderCode, ShaderType shaderType);
	bool linkShaders(const std::vector<uint32>& shaderHandlers);

	void populateAttributes();
	void populateUniforms();

	unsigned int handle_;
	std::map<std::string, unsigned int> attributes_;
	std::map<std::string, unsigned int> uniforms_;
};


#endif	//VENGINE3D_VE_SHADER_H
