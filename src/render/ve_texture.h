//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_TEXTURE_H
#define VENGINE3D_VE_TEXTURE_H

namespace VE{
    class Texture {
    public:
        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture();
        Texture(const char* path);
        ~Texture();

        void load(const char* path);
        void set(unsigned int uniformIndex, unsigned int textureIndex);
        void unSet(unsigned int textureIndex);
        unsigned int getHandle();

    protected:
        unsigned int width_;
        unsigned int height_;
        unsigned int channels_;
        unsigned int handle_;
    };
}



#endif //VENGINE3D_VE_TEXTURE_H
