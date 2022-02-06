//
// Created by boris on 2/6/2022.
//

#ifndef VENGINE3D_VE_SHADER_H
#define VENGINE3D_VE_SHADER_H
#include "stdlibs.h"

namespace VE {
    class Shader {
    public:
        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        Shader();
        ~Shader();
        Shader(const std::string& vertex, const std::string& fragment);
        void Load(const std::string& vertex, const std::string& fragment);

        void Bind();
        void UnBind();

        unsigned int getAttribute(const std::string& name);
        unsigned int getUniform(const std::string& name);
        unsigned int getHandle();

    private:

        std::string readFile(const std::string& path);
        unsigned int compileVertexShade(const std::string& vertex);
        unsigned int compileFragmentShader(const std::string& fragment);
        bool linkShaders(unsigned int vertex, unsigned int fragment);


        void populateAttributes();
        void populateUniforms();

        unsigned int handle_;
        std::map<std::string, unsigned int> attributes_;
        std::map<std::string, unsigned int> uniforms_;
    };
}


#endif //VENGINE3D_VE_SHADER_H
