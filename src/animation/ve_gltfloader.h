//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_GLTFLOADER_H
#define VENGINE3D_VE_GLTFLOADER_H

#include "cgltf.h"
#include "stdlibraries.h"

namespace VE {
    class GLTFFile {
    public:
        GLTFFile(const char *path) {
            cgltf_options options{};
            data_ = NULL;
            cgltf_result result = cgltf_parse_file(&options, path, &data_);

            if (result != cgltf_result_success) {
                data_ = NULL;
                std::cout << "Could not load: " << path << std::endl;
                return;
            }

            result = cgltf_load_buffers(&options, data_, path);
            if (result != cgltf_result_success) {
                cgltf_free(data_);
                data_ = NULL;
                std::cout << "Could not load: " << path << std::endl;
                return;
            }

            result = cgltf_validate(data_);
            if (result != cgltf_result_success) {
                cgltf_free(data_);
                data_ = NULL;
                std::cout << "Invalid file: " << path << std::endl;
                return;
            }
        };

        ~GLTFFile() {
            if(data_ != NULL) {
                cgltf_free(data_);
                data_ = NULL;
            }
        }

        bool good(){
            return data_ != NULL;
        }

        cgltf_data *data() {
            return data_;
        }

    private:
        cgltf_data *data_;
    };

}
#endif //VENGINE3D_VE_GLTFLOADER_H
