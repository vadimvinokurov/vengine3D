//
// Created by boris on 2/12/2022.
//

#include "ve_gltfloader.h"

VE::Pose VE::GLTF::loadRestPose() {
    std::size_t jointCount = data_->nodes_count;
    Pose result(jointCount);
    for (std::size_t i = 0; i < jointCount; ++i) {
        const cgltf_node &joint = data_->nodes[i];
        Transform transform = getLocalTransform(joint);
        result.setLocalTransform(i, transform);
        int parent = getNodeIndex(joint.parent, data_->nodes, jointCount);
        result.setParent(i, parent);
    }
    return result;
}

std::vector<VE::Clip> VE::GLTF::loadAnimationClips() {
    std::size_t numClips = data_->animations_count;

    std::vector<Clip> clips(numClips);
    for (std::size_t i = 0; i < numClips; ++i) {
        clips[i].setName(data_->animations[i].name);
        std::size_t numChannels = data_->animations[i].channels_count;

        for (std::size_t j = 0; j < numChannels; ++j) {
            cgltf_animation_channel &channel = data_->animations[i].channels[j];
            std::size_t nodeId = getNodeIndex(channel.target_node, data_->nodes, data_->nodes_count);

            if (channel.target_path == cgltf_animation_path_type_translation) {
                trackFromChannel<Vector3>(clips[i][nodeId].position, channel);
            } else if (channel.target_path == cgltf_animation_path_type_scale) {
                trackFromChannel<Vector3>(clips[i][nodeId].scale, channel);
            } else if (channel.target_path == cgltf_animation_path_type_rotation) {
                trackFromChannel<Quaternion>(clips[i][nodeId].rotation, channel);
            }
        }
        clips[i].recalculateDuration();
    }
    return clips;
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

std::size_t VE::GLTF::getNodeIndex(cgltf_node *target, cgltf_node *allNodes, std::size_t numNodes) {
    if (target == 0) {
        return Joint::hasNoParent;
    }
    for (std::size_t i = 0; i < numNodes; ++i) {
        if (target == &allNodes[i]) {
            return i;
        }
    }
    return Joint::hasNoParent;
}

template<typename T>
void VE::GLTF::trackFromChannel(VE::Track<T> &track, const cgltf_animation_channel &channel) {
    cgltf_animation_sampler &sampler = *channel.sampler;
    Interpolation interpolation = Interpolation::Constant;
    if (sampler.interpolation == cgltf_interpolation_type_linear) {
        interpolation = Interpolation::Linear;
    } else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) {
        interpolation = Interpolation::Cubic;
    }
    track.setInterpolation(interpolation);

    bool isSamplerCubic = interpolation == Interpolation::Cubic;

    std::vector<float> time = getScalarValues(1, *sampler.input);
    std::vector<float> val = getScalarValues(Frame<T>::N, *sampler.output);

    std::size_t numFrames = sampler.input->count;
    std::size_t compCount = val.size() / time.size();
    track.resize(numFrames);
    for (std::size_t i = 0; i < numFrames; ++i) {
        int baseIndex = i * compCount;
        Frame<T> &frame = track[i];
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

std::vector<float> VE::GLTF::getScalarValues(unsigned int compCount, const cgltf_accessor &inAccessor) {
    std::vector<float> out(inAccessor.count * compCount);
    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
    }
    return out;
}

std::vector<std::string> VE::GLTF::loadJointNames(cgltf_data *data) {
    std::size_t jointCount = data->nodes_count;
    std::vector<std::string> result(jointCount, "Not Set");
    for (std::size_t i = 0; i < jointCount; ++i) {
        cgltf_node *node = &(data->nodes[i]);
        if (node->name == 0) {
            result[i] = "EMPTY NODE";
        } else {
            result[i] = node->name;
        }
    }
    return result;
}


