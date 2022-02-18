//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_GLTFLOADER_H
#define VENGINE3D_VE_GLTFLOADER_H

#include "ve_core.h"

#include "cgltf.h"
#include "math/ve_transform.h"
#include "ve_track.h"
#include "ve_clip.h"

namespace VE {
    class GLTF {
    public:
        GLTF(const char *path) {
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

        ~GLTF() {
            if (data_ != NULL) {
                cgltf_free(data_);
                data_ = NULL;
            }
        }

        bool good() {
            return data_ != NULL;
        }

        cgltf_data *data() {
            return data_;
        }

        std::vector<Clip> loadAnimationClips();
        Pose loadRestPose();
        static Transform getLocalTransform(const cgltf_node &node);
        static std::size_t getNodeIndex(cgltf_node *target, cgltf_node *allNodes, std::size_t numNodes);
        template<typename T>
        static void trackFromChannel(Track<T> &track, const cgltf_animation_channel &channel);
        template<typename T>
        static std::vector<T> getValues(const cgltf_accessor &inAccessor);
        static std::vector<std::string> loadJointNames(cgltf_data *data);

    private:


        cgltf_data *data_;

    };
}
#endif //VENGINE3D_VE_GLTFLOADER_H
