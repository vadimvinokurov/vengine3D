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
#include "ve_skeleton.h"
#include "ve_mesh.h"
#include "span.h"

namespace VE {
	class GLTF {
	public:
		GLTF(const char* path);
		~GLTF();
		bool good();

		std::vector<Clip> loadAnimationClips();
		Pose loadRestPose();
		Pose loadBindPose();
		std::vector<std::string> loadJointNames();
		Skeleton loadSkeleton();
		std::vector<Mesh> loadMeshes();

	private:
		void meshFromAttribute(VE::Mesh& mesh, const cgltf_attribute& attribute, cgltf_skin* skin);
		Transform getLocalTransform(const cgltf_node& node);
		std::size_t getNodeIndex(const cgltf_node* target);
		template<typename T>
		void trackFromChannel(Track<T>& track, const cgltf_animation_channel& channel);
		template<typename T>
		std::vector<T> getAccessorValues(const cgltf_accessor& inAccessor);


		cgltf_data* data_;
		std::unordered_map<const cgltf_node*, size_t> nodeIndexes;
		tcb::span<cgltf_node> nodes;
	};
}  // namespace VE
#endif	//VENGINE3D_VE_GLTFLOADER_H
