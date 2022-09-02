//
// Created by boris on 8/19/2022.
//

#ifndef VENGINE3D_ASSETIMPORTER_H
#define VENGINE3D_ASSETIMPORTER_H

#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "span.h"
#include "AssetTypeTraits.h"

#include "Transform.h"
#include "Bone.h"
#include "Skeleton.h"
#include "StaticMesh.h"
#include "AnimTransformTrack.h"

using Weights = std::map<int32, float>;
using VertexWeights = std::map<uint32, Weights>;

class StaticMesh;
class SkeletalMeshModel;

class AssetImporter
{
public:
	AssetImporter(const char *path);
	std::vector<StaticMesh> loadMeshes();
	SkeletalMeshModel getSkeletalMesh();
	Skeleton getSkeleton();
	void getAnimations();

private:
	void boneIndexing();
	void boneIndexingByNode(aiNode *node, int32 &index);
	VertexWeights getMeshVertexWeights(const aiMesh *mesh);
	std::pair<IVector4, Vector4> weightsToEngineType(const Weights &weights);

	void loadBones(std::vector<Bone> &bones, aiNode *node, int32 parentId);
	void loadAnimation(const aiAnimation *animation);
	AnimTransformTrack loadChannel(const aiNodeAnim *channel, float tickPerSecond);

	template <class T>
	auto loadTrack(const T *aiKeys, uint32 size, float tickPerSecond);

	AnimFrame<Vector3> toAnimFrame(const aiVectorKey &key, float tickPerSecond);
	AnimFrame<Quaternion> toAnimFrame(const aiQuatKey &key, float tickPerSecond);

	static Transform getTransform(const aiMatrix4x4 &m);
	const aiScene *pScene = nullptr;
	Assimp::Importer Importer;
	std::unordered_map<std::string, int32> boneIndexMap;
};

template <class T>
auto AssetImporter::loadTrack(const T *aiKeys, uint32 size, float tickPerSecond)
{
	using EngineValueType = AssetSameType_t<decltype(aiKeys->mValue)>;

	tcb::span<const T> keysSrc(aiKeys, size);
	std::vector<AnimFrame<EngineValueType>> keys;
	keys.reserve(keysSrc.size());

	float lastTime = -1.0f;
	for (const auto &k : keysSrc)
	{
		keys.push_back(toAnimFrame(k, tickPerSecond));
		if (keys.back().time <= lastTime)
		{
			assert(false && "Incorrect animation track. Frame time less then previous frame.");
			keys.resize(1);
			return AnimTrack(keys);
		}
		lastTime = keys.back().time;
	}
	return AnimTrack(std::move(keys));
}

#endif // VENGINE3D_ASSETIMPORTER_H
