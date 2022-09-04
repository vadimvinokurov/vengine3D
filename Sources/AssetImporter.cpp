//
// Created by boris on 8/19/2022.
//

#include "AssetImporter.h"
#include "SkeletalMeshModel.h"
#include <sstream>

AssetImporter::AssetImporter(const char *path)
{
	pScene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
										 aiProcess_JoinIdenticalVertices);
	if (!pScene)
	{
		spdlog::error("Can't open asset file.");
		std::exit(1);
	}
	boneIndexing();
}

void AssetImporter::boneIndexing()
{
	int32 index = 0;
	boneIndexingByNode(pScene->mRootNode, index);
}

void AssetImporter::boneIndexingByNode(aiNode *node, int32 &index)
{
	boneIndexMap[node->mName.C_Str()] = index++;
	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		boneIndexingByNode(node->mChildren[i], index);
	}
}

SkeletalMeshModel AssetImporter::getSkeletalMesh()
{
	SkeletalMeshModel skeletalMeshModel;
	tcb::span<aiMesh *> meshes(pScene->mMeshes, pScene->mNumMeshes);
	for (const auto &mesh : meshes)
	{
		SkeletalMeshSection skeletalMeshSection;
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		for (uint32 i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D *pPos = &(mesh->mVertices[i]);
			const aiVector3D *pNormal = &(mesh->mNormals[i]);
			const aiVector3D *pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
			skeletalMeshSection.vertices.emplace_back(pPos->x, pPos->y, pPos->z);
			skeletalMeshSection.normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
			skeletalMeshSection.textureCoordinates.emplace_back(pTexCoord->x, pTexCoord->y);
		}

		for (uint32 i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace &Face = mesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			skeletalMeshSection.indices.push_back(Face.mIndices[0]);
			skeletalMeshSection.indices.push_back(Face.mIndices[1]);
			skeletalMeshSection.indices.push_back(Face.mIndices[2]);
		}

		skeletalMeshSection.influenceBones.resize(skeletalMeshSection.vertices.size(), IVector4(INVALID_BODE_ID));
		skeletalMeshSection.influenceWeights.resize(skeletalMeshSection.vertices.size(), Vector4(0.0f));

		VertexWeights vertexWeights = getMeshVertexWeights(mesh);

		for (const auto &weights : vertexWeights)
		{
			uint32 vertex = weights.first;
			auto [bonesVector, weightsVector] = weightsToEngineType(weights.second);
			skeletalMeshSection.influenceBones.at(vertex) = bonesVector;
			skeletalMeshSection.influenceWeights.at(vertex) = weightsVector;
		}

		skeletalMeshModel.addSkeletalMeshSection(skeletalMeshSection);
	}
	return skeletalMeshModel;
}

VertexWeights AssetImporter::getMeshVertexWeights(const aiMesh *mesh)
{
	VertexWeights vertexWeights;
	tcb::span<aiBone *> meshBones(mesh->mBones, mesh->mNumBones);
	for (const auto &bone : meshBones)
	{
		int32 boneId = boneIndexMap[bone->mName.C_Str()];
		tcb::span<aiVertexWeight> weights(bone->mWeights, bone->mNumWeights);
		for (const auto &weight : weights)
		{
			vertexWeights[weight.mVertexId][boneId] = weight.mWeight;
		}
	}
	return vertexWeights;
}

std::pair<IVector4, Vector4> AssetImporter::weightsToEngineType(const Weights &weights)
{
	std::multimap<float, uint32, std::greater<float>> sortedWeights;
	for (const auto &weight : weights)
	{
		if (weight.first == INVALID_BODE_ID || weight.second == 0.0f)
		{
			continue;
		}
		sortedWeights.emplace(weight.second, weight.first);
	}

	Vector4 weightsVector(0.0f);
	IVector4 bonesVector(INVALID_BODE_ID);

	uint32 index = 0;
	for (const auto &sortedWeight : sortedWeights)
	{
		if (index >= 4)
		{
			break;
		}
		weightsVector.v[index] = sortedWeight.first;
		bonesVector.v[index] = sortedWeight.second;
		++index;
	}

	float totalWeight = weightsVector.v[0] + weightsVector.v[1] + weightsVector.v[2] + weightsVector.v[3];

	if (totalWeight < 0.8f || totalWeight > 1.1f)
	{
		assert(false && "Incorrect weight. VEngine support only 4 bone per vertex.");
		spdlog::warn("Incorrect weight : {}", totalWeight);
		std::stringstream s;
		for (const auto &swb : sortedWeights)
		{
			s << "b" << swb.second << " w" << swb.first;
		}
		s << std::endl;
		spdlog::warn("  {}", s.str());
	}

	return {bonesVector, weightsVector};
}

Skeleton AssetImporter::getSkeleton()
{
	std::vector<Bone> bones(boneIndexMap.size());
	loadBones(bones, pScene->mRootNode, INVALID_BODE_ID);

	tcb::span<aiMesh *> meshes(pScene->mMeshes, pScene->mNumMeshes);
	for (const auto &mesh : meshes)
	{
		tcb::span<aiBone *> meshBones(mesh->mBones, mesh->mNumBones);
		for (const auto &bone : meshBones)
		{
			int32 boneId = boneIndexMap[bone->mName.C_Str()];
			bones[boneId].offset = getTransform(bone->mOffsetMatrix);
		}
	}
	Skeleton skeleton(bones);
	skeleton.setGlobalInverseTransform(getTransform(pScene->mRootNode->mTransformation).getInversed());
	return skeleton;
}

void AssetImporter::loadBones(std::vector<Bone> &bones, aiNode *node, int32 parentId)
{
	int32 boneId = boneIndexMap[std::string(node->mName.C_Str())];
	bones[boneId].transform = getTransform(node->mTransformation);
	bones[boneId].parentId = parentId;
	bones[boneId].name = node->mName.C_Str();

	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		loadBones(bones, node->mChildren[i], boneId);
	}
}

Transform AssetImporter::getTransform(const aiMatrix4x4 &m)
{
	aiVector3D aiscale;
	aiVector3D aiposition;
	aiQuaternion airotation;
	m.Decompose(aiscale, airotation, aiposition);
	Transform transform;
	transform.scale = Vector3(aiscale.x, aiscale.y, aiscale.z);
	transform.position = Vector3(aiposition.x, aiposition.y, aiposition.z);
	transform.rotation = Quaternion(airotation.x, airotation.y, airotation.z, airotation.w);
	return transform;
}

std::vector<StaticMesh> AssetImporter::loadMeshes()
{
	std::vector<StaticMesh> meshes;
	// spdlog::warn("Mesh count: {}", pScene->mNumMeshes);
	tcb::span<aiMesh *> meshesInFile(pScene->mMeshes, pScene->mNumMeshes);
	for (const auto &paiMesh : meshesInFile)
	{
		meshes.emplace_back();
		StaticMesh &mesh = meshes.back();
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			const aiVector3D *pPos = &(paiMesh->mVertices[i]);
			const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
			mesh.vertices.emplace_back(pPos->x, pPos->y, pPos->z);
			mesh.normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
			mesh.textureCoordinates.emplace_back(pTexCoord->x, pTexCoord->y);
		}

		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
		{
			const aiFace &Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			mesh.indices.push_back(Face.mIndices[0]);
			mesh.indices.push_back(Face.mIndices[1]);
			mesh.indices.push_back(Face.mIndices[2]);
		}
	}
	return meshes;
}
std::unordered_map<std::string, Animation> AssetImporter::getAnimations()
{
	std::unordered_map<std::string, Animation> animations;

	tcb::span<aiAnimation *> animationsSrc(pScene->mAnimations, pScene->mNumAnimations);
	for (const auto &anim : animationsSrc)
	{
		animations.emplace(anim->mName.C_Str(), loadAnimation(anim));
	}
	return animations;
}

Animation AssetImporter::loadAnimation(const aiAnimation *animationSrc)
{
	float durationInSec = animationSrc->mDuration / animationSrc->mTicksPerSecond;
	Animation animation(animationSrc->mName.C_Str(), durationInSec);

	tcb::span<aiNodeAnim *> channels(animationSrc->mChannels, animationSrc->mNumChannels);
	for (const auto &channel : channels)
	{
		animation.addTransformTrack(loadChannel(channel, animationSrc->mTicksPerSecond));
	}
	return animation;
}

AnimTransformTrack AssetImporter::loadChannel(const aiNodeAnim *channel, float tickPerSecond)
{
	auto positionTrack = loadTrack(channel->mPositionKeys, channel->mNumPositionKeys, tickPerSecond);
	auto rotationTrack = loadTrack(channel->mRotationKeys, channel->mNumRotationKeys, tickPerSecond);
	auto scalingTrack = loadTrack(channel->mScalingKeys, channel->mNumScalingKeys, tickPerSecond);

	AnimTransformTrack transformTrack(boneIndexMap[channel->mNodeName.C_Str()]);
	transformTrack.setPositionTrack(positionTrack);
	transformTrack.setRotationTrack(rotationTrack);
	transformTrack.setScalingTrack(scalingTrack);
	return transformTrack;
}

AnimFrame<Vector3> AssetImporter::toAnimFrame(const aiVectorKey &key, float tickPerSecond)
{
	AnimFrame<Vector3> frame;
	frame.time = key.mTime / tickPerSecond;
	frame.value = Vector3(key.mValue.x, key.mValue.y, key.mValue.z);
	return frame;
}

AnimFrame<Quaternion> AssetImporter::toAnimFrame(const aiQuatKey &key, float tickPerSecond)
{
	AnimFrame<Quaternion> frame;
	frame.time = key.mTime / tickPerSecond;
	frame.value = Quaternion(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w);
	return frame;
}
