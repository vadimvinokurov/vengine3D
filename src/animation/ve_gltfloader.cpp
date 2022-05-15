//
// Created by boris on 2/12/2022.
//

#include "ve_gltfloader.h"

VE::GLTF::GLTF(const char* path) {
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

	nodes = tcb::span(data_->nodes, data_->nodes_count);
	size_t nodeIndex = 0;
	nodeIndexes.emplace(nullptr, -1);
	for (const auto& node : nodes) { nodeIndexes.emplace(&node, nodeIndex++); }
}

VE::GLTF::~GLTF() {
	if (data_ != NULL) {
		cgltf_free(data_);
		data_ = NULL;
	}
}

bool VE::GLTF::good() {
	return data_ != NULL;
}

std::vector<VE::Mesh> VE::GLTF::loadMeshes() {
	std::vector<VE::Mesh> meshes;

	for (const auto& node : nodes) {
		if (node.mesh == 0 || node.skin == 0) continue;

		tcb::span<cgltf_primitive> primitives(node.mesh->primitives, node.mesh->primitives_count);
		for (const auto& primitive : primitives) {
			meshes.emplace_back();
			Mesh& mesh = meshes.back();

			tcb::span<cgltf_attribute> attributes(primitive.attributes, primitive.attributes_count);
			for (const auto& attribute : attributes) { meshFromAttribute(mesh, attribute, node.skin); }

			if (primitive.indices != 0) {
				mesh.indices.reserve(primitive.indices->count);
				mesh.indices.clear();
				for (std::size_t k = 0; k < primitive.indices->count; ++k) { mesh.indices.emplace_back(cgltf_accessor_read_index(primitive.indices, k)); }
			}
			mesh.updateOpenGLBuffers();
		}
	}

	return meshes;
}

VE::Skeleton VE::GLTF::loadSkeleton() {
	return VE::Skeleton(loadRestPose(), loadBindPose(), loadJointNames());
}

VE::Pose VE::GLTF::loadRestPose() {
	Pose pose;
	for (std::size_t i = 0; i < nodes.size(); ++i) { pose.addJoint(i, getLocalTransform(nodes[i]), getNodeIndex(nodes[i].parent)); }
	return pose;
}

VE::Pose VE::GLTF::loadBindPose() {
	Pose restPose = loadRestPose();
	std::vector<Transform> worldBindPose(restPose.jointsCount());

	for (std::size_t i = 0; i < restPose.jointsCount(); ++i) { worldBindPose[i] = restPose.getGlobalTransform(i); }

	tcb::span skins(data_->skins, data_->skins_count);
	for (const auto& skin: skins) {
		auto invBindMatrix = getAccessorValues<Matrix4>(*skin.inverse_bind_matrices);
		for (std::size_t j = 0; j < skin.joints_count; ++j) {
			auto jointIndex = getNodeIndex(skin.joints[j]);
			worldBindPose[jointIndex] = Transform::fromMatrix(invBindMatrix[j].getInversed());
		}
	}

	Pose bindPose = std::move(restPose);
	for (std::size_t i = 0; i < bindPose.jointsCount(); ++i) {
		Transform currentTransform = worldBindPose[i];
		auto parentIndex = bindPose.getParentIndex(i);
		if (parentIndex != Joint::hasNoParent) {
			Transform parentTransform = worldBindPose[parentIndex];
			currentTransform = parentTransform.getInversed() * currentTransform;
		}
		bindPose[i] = currentTransform;
	}
	return bindPose;
}


std::vector<std::string> VE::GLTF::loadJointNames() {
	std::vector<std::string> result;
	for (const auto& node : nodes) { result.emplace_back(node.name ? node.name : "no name"); }
	return result;
}

std::vector<VE::Clip> VE::GLTF::loadAnimationClips() {
	std::vector<Clip> clips(data_->animations_count);
	for (std::size_t i = 0; i < clips.size(); ++i) {
		clips[i].setName(data_->animations[i].name);
		tcb::span animationChannels(data_->animations[i].channels, data_->animations[i].channels_count);
		for (const auto& channel : animationChannels) {
			std::size_t nodeIndex = getNodeIndex(channel.target_node);

			switch (channel.target_path) {
				case cgltf_animation_path_type_translation: clips[i][nodeIndex].position = trackFromChannel<Vector3>(*channel.sampler); break;
				case cgltf_animation_path_type_scale: clips[i][nodeIndex].scale = trackFromChannel<Vector3>(*channel.sampler); break;
				case cgltf_animation_path_type_rotation: clips[i][nodeIndex].rotation = trackFromChannel<Quaternion>(*channel.sampler); break;
				default: break;
			}
		}

		clips[i].recalculateDuration();
	}
	return clips;
}

VE::Transform VE::GLTF::getLocalTransform(const cgltf_node& node) {
	Transform result;
	if (node.has_matrix) { result = Transform::fromMatrix(Matrix4(node.matrix)); }
	if (node.has_translation) { result.position = Vector3(node.translation); }
	if (node.has_rotation) { result.rotation = Quaternion(node.rotation); }
	if (node.has_scale) { result.scale = Vector3(node.scale); }
	return result;
}

std::size_t VE::GLTF::getNodeIndex(const cgltf_node* target) {
	return nodeIndexes.at(target);
}

template<typename T>
VE::Track<T> VE::GLTF::trackFromChannel(const cgltf_animation_sampler& sampler) {

	Interpolation interpolation = Interpolation::Constant;
	if (sampler.interpolation == cgltf_interpolation_type_linear) {
		interpolation = Interpolation::Linear;
	} else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) {
		interpolation = Interpolation::Cubic;
	}
	bool isSamplerCubic = interpolation == Interpolation::Cubic;

	std::vector<float> time = getAccessorValues<float>(*sampler.input);
	std::vector<T> values = getAccessorValues<T>(*sampler.output);
	std::size_t numberOfValuesPerFrame = values.size() / time.size();

	std::vector<Frame<T>> frames(sampler.input->count);
	for (std::size_t i = 0; i < frames.size(); ++i) {
		std::size_t baseIndex = i * numberOfValuesPerFrame;
		frames[i].time = time[i];
		frames[i].in = isSamplerCubic ? values[baseIndex++] : T();
		frames[i].value = values[baseIndex++];
		frames[i].out = isSamplerCubic ? values[baseIndex] : T();
	}
	return Track(std::move(frames), interpolation);
}

void VE::GLTF::meshFromAttribute(VE::Mesh& mesh, const cgltf_attribute& attribute, cgltf_skin* skin) {
	cgltf_attribute_type attributeType = attribute.type;
	cgltf_accessor& accessor = *attribute.data;

	switch (attributeType) {
		case cgltf_attribute_type_position: {
			mesh.positions = getAccessorValues<Vector3>(accessor);
			return;
		}
		case cgltf_attribute_type_normal: {
			mesh.normals = getAccessorValues<Vector3>(accessor);
			for (auto&& normal : mesh.normals) {
				if (normal.lenSqrt() < VEngineSettings::VECTOR_EPSILON) {
					normal = Vector3(0, 0, 1);
				} else {
					normal.normalize();
				}
			}
			return;
		}
		case cgltf_attribute_type_texcoord: {
			mesh.textureCoordinates = getAccessorValues<Vector2>(accessor);
			return;
		}
		case cgltf_attribute_type_weights: {
			mesh.jointsInfluencesWeights = getAccessorValues<Vector4>(accessor);
			return;
		}
		case cgltf_attribute_type_joints: {
			auto jointsIndexFloat = getAccessorValues<Vector4>(accessor);
			mesh.jointsInfluences.resize(jointsIndexFloat.size());
			for (std::size_t i = 0; i < jointsIndexFloat.size(); i++) {
				IVector4 joint(jointsIndexFloat[i].x + 0.5f, jointsIndexFloat[i].y + 0.5f, jointsIndexFloat[i].z + 0.5f, jointsIndexFloat[i].w + 0.5f);

				joint.x = std::max<int>(0, getNodeIndex(skin->joints[joint.x]));
				joint.y = std::max<int>(0, getNodeIndex(skin->joints[joint.y]));
				joint.z = std::max<int>(0, getNodeIndex(skin->joints[joint.z]));
				joint.w = std::max<int>(0, getNodeIndex(skin->joints[joint.w]));

				mesh.jointsInfluences[i] = joint;
			}
			return;
		}
		case cgltf_attribute_type_invalid: return;
		case cgltf_attribute_type_tangent: return;
		case cgltf_attribute_type_color: return;
	}
	return;
}

template<typename T>
std::vector<T> VE::GLTF::getAccessorValues(const cgltf_accessor& inAccessor) {
	std::vector<T> out(inAccessor.count);

	for (cgltf_size i = 0; i < inAccessor.count; ++i) {
		if constexpr (std::is_same_v<T, float>) {
			cgltf_accessor_read_float(&inAccessor, i, &out[i], 1);
		} else {
			cgltf_accessor_read_float(&inAccessor, i, out[i].data(), T::size());
		}
	}
	return out;
}
