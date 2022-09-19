//
// Created by boris on 2/6/2022.
//
#include "Shader.h"
#include "Render/opengl_glad.h"

static auto ShaderTypeToOGL(ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		return GL_VERTEX_SHADER;
	case ShaderType::Fragment:
		return GL_FRAGMENT_SHADER;
	default:
		return GL_VERTEX_SHADER;
	}
}

Shader::Shader(const std::vector<ShaderSource> &shaderSources)
{
	handle_ = glCreateProgram();

	std::vector<uint32> shaderHanlers;

	for (const auto &[sourceFile, type] : shaderSources)
	{
		std::string source = readFile(sourceFile);
		auto handle = compileShade(source, type);
		shaderHanlers.push_back(handle);
	}

	linkShaders(shaderHanlers);
	populateAttributes();
	populateUniforms();
	spdlog::info("Shader compiled successfully!");
}

Shader::~Shader()
{
	glDeleteProgram(handle_);
}

void Shader::bind()
{
	glUseProgram(handle_);
}

void Shader::unBind()
{
	glUseProgram(0);
}

uint32 Shader::getAttribute(const std::string &name)
{
	auto it = attributes_.find(name);
	if (it == attributes_.end())
	{
		spdlog::error("Bad shader attribute index: " + name);
		std::exit(1);
	}
	return it->second;
}

uint32 Shader::getUniform(const std::string &name)
{
	auto it = uniforms_.find(name);
	if (it == uniforms_.end())
	{
		spdlog::error("Bad shader uniforms index: " + name);
		std::exit(1);
	}
	return it->second;
}

unsigned int Shader::getHandle()
{
	return handle_;
}

void Shader::populateAttributes()
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;

	glUseProgram(handle_);
	glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTES, &count);

	for (int i = 0; i < count; ++i)
	{
		memset(name, 0, sizeof(char) * 128);
		glGetActiveAttrib(handle_, (GLuint)i, 128, &length, &size, &type, name);
		int attrib = glGetAttribLocation(handle_, name);
		if (attrib >= 0)
		{
			attributes_[name] = attrib;
		}
	}
	glUseProgram(0);
}

void Shader::populateUniforms()
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;
	char testName[256];

	glUseProgram(handle_);
	glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; ++i)
	{
		memset(name, 0, sizeof(char) * 128);
		glGetActiveUniform(handle_, (GLuint)i, 128, &length, &size, &type, name);
		int unifrom = glGetUniformLocation(handle_, name);
		if (unifrom >= 0)
		{
			std::string uniformName = name;
			auto found = uniformName.find('[');
			if (found != std::string::npos)
			{
				uniformName.erase(uniformName.begin() + found, uniformName.end());

				// unsigned int uniformIndex = 0;
				while (true)
				{
					memset(testName, 0, sizeof(char) * 256);
					// sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
					int uniformLocation = glGetUniformLocation(handle_, testName);
					if (uniformLocation < 0)
					{
						break;
					}
					uniforms_[testName] = uniformLocation;
				}
			}
			uniforms_[uniformName] = unifrom;
		}
	}
	glUseProgram(0);
}

std::string Shader::readFile(const std::string &path)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		spdlog::error("Shader source file not found.");
		std::exit(1);
	}
	std::stringstream contents;
	contents << file.rdbuf();
	return contents.str();
}

uint32 Shader::compileShade(const std::string &shaderCode, ShaderType shaderType)
{
	uint32 shaderHandle = glCreateShader(ShaderTypeToOGL(shaderType));
	const char *source = shaderCode.c_str();

	glShaderSource(shaderHandle, 1, &source, NULL);
	glCompileShader(shaderHandle);
	int success = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
		glDeleteShader(shaderHandle);
		spdlog::critical("Shader compilation failed. " + std::string(infoLog));
		std::exit(1);
	}
	return shaderHandle;
}

bool Shader::linkShaders(const std::vector<uint32> &shaderHandlers)
{
	for (auto handle : shaderHandlers)
	{
		glAttachShader(handle_, handle);
	}
	glLinkProgram(handle_);

	int success = 0;
	glGetProgramiv(handle_, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(handle_, 512, NULL, infoLog);
		for (auto handle : shaderHandlers)
		{
			glDeleteShader(handle);
		}
		spdlog::critical("Shader linking failed. " + std::string(infoLog));
		std::exit(1);
	}
	for (auto handle : shaderHandlers)
	{
		glDeleteShader(handle);
	}
	return true;
}