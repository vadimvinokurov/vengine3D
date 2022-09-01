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

#include "Transform.h"
#include "Bone.h"
#include "Skeleton.h"
#include "StaticMesh.h"

using Weights = std::map<int32, float>;
using VertexWeights = std::map<uint32, Weights>;

class StaticMesh;
class SkeletalMeshModel;

class AssetImporter {
public:
	AssetImporter(const char* path);
	std::vector<StaticMesh> loadMeshes();
	SkeletalMeshModel getSkeletalMesh();
	Skeleton getSkeleton();
private:
	void boneIndexing();
	void boneIndexingByNode(aiNode* node, int32& index);
	VertexWeights getMeshVertexWeights(const aiMesh *mesh);
	std::pair<IVector4, Vector4> weightsToEngineType(const Weights& weights);

	void loadBones(std::vector<Bone>& bones, aiNode* node, int32 parentId);
	static Transform getTransform(const aiMatrix4x4& m);
	const aiScene* pScene = nullptr;
	Assimp::Importer Importer;
	std::unordered_map<std::string, int32> boneIndexMap;
};


#endif // VENGINE3D_ASSETIMPORTER_H
