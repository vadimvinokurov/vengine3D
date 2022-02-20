//
// Created by boris on 2/12/2022.
//

#include "ve_gltfloader.h"

VE::Skeleton VE::GLTF::loadSkeleton() {
    return VE::Skeleton(loadRestPose(), loadBindPose(), loadJointNames());
}

VE::Pose VE::GLTF::loadRestPose() {
    std::size_t jointCount = data_->nodes_count;
    Pose result(jointCount);
    for (std::size_t i = 0; i < jointCount; ++i) {
        const cgltf_node &joint = data_->nodes[i];
        Transform transform = getLocalTransform(joint);
        result.setLocalTransform(i, transform);
        auto parentIndex = getNodeIndex(joint.parent, data_->nodes, jointCount);
        result.setParent(i, parentIndex);
    }
    return result;
}

VE::Pose VE::GLTF::loadBindPose() {
    Pose restPose = loadRestPose();
    auto numJoints = restPose.jointsCount();
    std::vector<Transform> worldBindPose(numJoints);

    for(std::size_t i = 0; i < numJoints; ++i) {
        worldBindPose[i] = restPose.getGlobalTransform(i);
    }

    auto numSkins = data_->skins_count;
    for(std::size_t i = 0; i < numSkins; ++i){
        const cgltf_skin& skin = data_->skins[i];
        auto invBindMatrix = getValues<Matrix4>(*skin.inverse_bind_matrices);
        auto numSkinJoints = skin.joints_count;
        for(std::size_t j = 0; j < numSkinJoints; ++j) {
            auto jointIndex = getNodeIndex(skin.joints[j], data_->nodes, numJoints);
            worldBindPose[jointIndex] = Transform::fromMatrix(invBindMatrix[j].getInversed());;
        }
    }

    Pose bindPose = restPose;
    for(std::size_t i = 0; i < numJoints; ++i) {
        Transform currentTransform = worldBindPose[i];
        auto parentIndex = bindPose.getParentIndex(i);
        if(parentIndex != Joint::hasNoParent) {
            Transform parentTransform = worldBindPose[parentIndex];
            currentTransform = parentTransform.getInversed() * currentTransform;
        }
        bindPose.setLocalTransform(i, currentTransform);
    }
    return bindPose;
}

std::vector<std::string> VE::GLTF::loadJointNames() {
    std::size_t jointCount = data_->nodes_count;
    std::vector<std::string> result(jointCount, "Not Set");
    for (std::size_t i = 0; i < jointCount; ++i) {
        cgltf_node *node = &(data_->nodes[i]);
        if (node->name == 0) {
            result[i] = "EMPTY NODE";
        } else {
            result[i] = node->name;
        }
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

    std::vector<float> time = getValues<float>(*sampler.input);
    std::vector<T> values = getValues<T>(*sampler.output);

    std::size_t numFrames = sampler.input->count;
    std::size_t numberOfValuesPerFrame = values.size() / time.size();
    track.resize(numFrames);
    for (std::size_t i = 0; i < numFrames; ++i) {
        std::size_t baseIndex = i * numberOfValuesPerFrame;
        Frame<T> &frame = track[i];
        frame.time = time[i];
        frame.in = isSamplerCubic ? values[baseIndex++] : T(0.0f);
        frame.value = values[baseIndex++];
        frame.out = isSamplerCubic ? values[baseIndex] : T(0.0f);
    }
}

template<typename T>
std::vector<T> VE::GLTF::getValues(const cgltf_accessor &inAccessor) {
    std::vector<T> out(inAccessor.count);

    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, i, out[i].data(), T::size());
    }
    return out;
}

template<>
std::vector<float> VE::GLTF::getValues(const cgltf_accessor &inAccessor) {
    std::vector<float> out(inAccessor.count);

    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, i, &out[i], 1);
    }
    return out;
}

