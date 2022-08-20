//
// Created by boris on 2/6/2022.
//

#ifndef VENGINE3D_VE_SHADER_H
#define VENGINE3D_VE_SHADER_H

#include "EngineCore.h"

enum class ShaderType : uint8_t
{
	Vertex = 0,
	Fragment = 1
};

struct ShaderSource
{
	ShaderSource(const std::string &_sourceFile, ShaderType _type) : sourceFile(_sourceFile), type(_type)
	{
	}
	std::string sourceFile;
	ShaderType type;
};

class Shader
{
public:
	Shader(const std::vector<ShaderSource> &shaders);
	~Shader();
	Shader(const Shader &) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader(Shader &&other) = delete;
	Shader &operator=(Shader &&other) = delete;



	void bind();
	void unBind();

	uint32 getAttribute(const std::string &name);
	uint32 getUniform(const std::string &name);
	uint32 getHandle();

private:
	std::string readFile(const std::string &path);
	uint32 compileShade(const std::string &shaderCode, ShaderType shaderType);
	bool linkShaders(const std::vector<uint32> &shaderHandlers);

	void populateAttributes();
	void populateUniforms();

	std::map<std::string, uint32> attributes_;
	std::map<std::string, uint32> uniforms_;

	uint32 handle_;
};

using ShaderPtr = std::shared_ptr<Shader>;

#endif // VENGINE3D_VE_SHADER_H
