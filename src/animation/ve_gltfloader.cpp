//
// Created by boris on 2/12/2022.
//

#include "ve_gltfloader.h"

VE::Pose VE::GLTF::loadRestPose() {
    unsigned int boneCount = data_->nodes_count;
    Pose result(boneCount);
    for (unsigned int i = 0; i < boneCount; ++i) {
        const cgltf_node &joint = data_->nodes[i];
        Transform transform = getLocalTransform(joint);
        result.setLocalTransform(i, transform);
        int parent = getNodeIndex(joint.parent, data_->nodes, boneCount);
        result.setParent(i, parent);
    }
    return result;
}

std::vector<VE::Clip> VE::GLTF::loadAnimationClips() {
    unsigned int numClips = data_->animations_count;
    unsigned int numNodes = data_->nodes_count;

    std::vector<Clip> result(numClips);
    for (unsigned int i = 0; i < numClips; ++i) {
        result[i].setName(data_->animations[i].name);
        unsigned int numChannels = data_->animations[i].channels_count;
        for (unsigned int j = 0; j < numChannels; ++j) {
            cgltf_animation_channel &channel = data_->animations[i].channels[j];
            cgltf_node *target = channel.target_node;
            int nodeId = getNodeIndex(target, data_->nodes, numNodes);
            if (channel.target_path == cgltf_animation_path_type_translation) {
                trackFromChannel<Vector3>(result[i][nodeId].position, channel);
            } else if (channel.target_path == cgltf_animation_path_type_scale) {
                trackFromChannel<Vector3>(result[i][nodeId].scale, channel);
            } else if (channel.target_path == cgltf_animation_path_type_rotation) {
                trackFromChannel<Quaternion>(result[i][nodeId].rotation, channel);
            }
        }
        result[i].recalculateDuration();
    }
    return result;
}

VE::Transform VE::GLTF::getLocalTransform(const cgltf_node &node) {
    Transform result;
    if (node.has_matrix) {
        result = Transform::fromMatrix(Matrix4(node.matrix));
    }
    if (node.has_translation) {
        result.position = Vector3(node.translation);
    }
    if (node.has_rotation) {
        result.rotation = Quaternion(node.rotation);
    }
    return result;
}

int VE::GLTF::getNodeIndex(cgltf_node *target, cgltf_node *allNodes, unsigned int numNodes) {
    if (target == 0) {
        return -1;
    }
    for (unsigned int i = 0; i < numNodes; ++i) {
        if (target == &allNodes[i]) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void VE::GLTF::getScalarValues(std::vector<float> &out, unsigned int compCount, const cgltf_accessor &inAccessor) {
    out.resize(inAccessor.count * compCount);
    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
    }
}

std::vector<std::string> VE::GLTF::loadJointNames(cgltf_data *data) {
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
void VE::GLTF::trackFromChannel(VE::Track<T> &result, const cgltf_animation_channel &channel) {
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
            frame.in.v[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
        }
        for (int comp = 0; comp < Frame<T>::N; ++comp) {
            frame.value.v[comp] = val[baseIndex + offset++];
        }
        for (int comp = 0; comp < Frame<T>::N; ++comp) {
            frame.out.v[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
        }
    }
}


