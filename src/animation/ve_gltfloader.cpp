//
// Created by boris on 2/12/2022.
//

#include "ve_gltfloader.h"

void VE::GLTFFile::getScalarValues(std::vector<float> &out, unsigned int compCount, const cgltf_accessor &inAccessor) {
    out.resize(inAccessor.count * compCount);
    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
    }
}

VE::Transform VE::GLTFFile::getLocalTransform(cgltf_node &n) {
    Transform result;
    if (n.has_matrix) {
        Matrix4 mat(n.matrix);
        result = Transform::fromMatrix(mat);
    }
    if (n.has_translation) {
        result.position = Vector3(n.translation[0],
                                  n.translation[1], n.translation[2]);
    }
    if (n.has_rotation) {
        result.rotation = Quaternion(n.rotation[0],
                                     n.rotation[1], n.rotation[2], n.rotation[3]);
    }
    return result;
}

int VE::GLTFFile::getNodeIndex(cgltf_node *target, cgltf_node *allNodes, unsigned int numNodes) {
    if (target == 0) {
        return -1;
    }
    for (unsigned int i = 0; i < numNodes; ++i) {
        if (target == &allNodes[i]) {
            return (int) i;
        }
    }
    return -1;
}


std::vector<std::string> VE::GLTFFile::loadJointNames(cgltf_data *data) {
    unsigned int boneCount = static_cast<unsigned int>(data->nodes_count);
    std::vector<std::string> result(boneCount, "Not Set");
    for (unsigned int i = 0; i < boneCount; ++i) {
        cgltf_node *node = &(data->nodes[i]);
        if (node->name == 0) {
            result[i] = "EMPTY NODE";
        } else {
            result[i] = node->name;
        }
    }
    return result;
}

template<typename T>
void VE::GLTFFile::trackFromChannel(VE::Track<T> &result, const cgltf_animation_channel &channel) {
    cgltf_animation_sampler &sampler = *channel.sampler;
    Interpolation interpolation = Interpolation::Constant;
    if (sampler.interpolation == cgltf_interpolation_type_linear) {
        interpolation = Interpolation::Linear;
    } else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) {
        interpolation = Interpolation::Cubic;
    }
    bool isSamplerCubic = interpolation == Interpolation::Cubic;
    result.setInterpolation(interpolation);
    std::vector<float> time; // times
    getScalarValues(time, 1, *sampler.input);
    std::vector<float> val; // values
    getScalarValues(val, Frame<T>::N, *sampler.output);

    unsigned int numFrames = sampler.input->count;
    unsigned int compCount = val.size() / time.size();
    result.resize(numFrames);
    for (unsigned int i = 0; i < numFrames; ++i) {
        int baseIndex = i * compCount;
        Frame<T> &frame = result[i];
        int offset = 0;
        frame.time = time[i];
        for (int comp = 0; comp < Frame<T>::N; ++comp) {
            frame.in[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
        }
        for (int comp = 0; comp < Frame<T>::N; ++comp) {
            frame.value[comp] = val[baseIndex + offset++];
        }
        for (int comp = 0; comp < Frame<T>::N; ++comp) {
            frame.out[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
        }
    }
}

std::vector<VE::Clip> VE::GLTFFile::loadAnimationClips(cgltf_data *data) {
    unsigned int numClips = data->animations_count;
    unsigned int numNodes = data->nodes_count;
    std::vector<Clip> result;
    result.resize(numClips);
    for (unsigned int i = 0; i < numClips; ++i) {
        result[i].setName(data->animations[i].name);
        unsigned int numChannels = data->animations[i].channels_count;
        for (unsigned int j = 0; j < numChannels; ++j) {
            cgltf_animation_channel &channel = data->animations[i].channels[j];
            cgltf_node *target = channel.target_node;
            int nodeId = getNodeIndex(target, data->nodes, numNodes);
            if (channel.target_path == cgltf_animation_path_type_translation) {
                VE::Track<Vector3> &track = result[i][nodeId].getPositionTrack();
                trackFromChannel<Vector3>(track, channel);
            } else if (channel.target_path == cgltf_animation_path_type_scale) {
                VE::Track<Vector3> &track = result[i][nodeId].getScaleTrack();
                trackFromChannel<Vector3>(track, channel);
            } else if (channel.target_path == cgltf_animation_path_type_rotation) {
                VE::Track<Quaternion> &track = result[i][nodeId].getRotationTrack();
                trackFromChannel<Quaternion>(track, channel);
            }
        }
        result[i].recalculateDuration();
    }
    return result;
}

VE::Pose VE::GLTFFile::loadRestPose(cgltf_data *data) {
    unsigned int boneCount = data->nodes_count;
    Pose result(boneCount);
    for (unsigned int i = 0; i < boneCount; ++i) {
        cgltf_node *node = &(data->nodes[i]);
        Transform transform = getLocalTransform(data->nodes[i]);
        result.setLocalTransform(i, transform);
        int parent = getNodeIndex(node->parent, data->nodes, boneCount);
        result.setParent(i, parent);
    }
    return result;
}



