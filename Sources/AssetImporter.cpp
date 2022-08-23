//
// Created by boris on 8/19/2022.
//

#include "AssetImporter.h"
#include "span.h"
#include "SkeletalMesh.h"

AssetImporter::AssetImporter(const char *path)
{
	pScene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
										 aiProcess_JoinIdenticalVertices);
}

bool AssetImporter::good()
{
	return pScene;
}

std::vector<StaticMesh> AssetImporter::loadMeshes()
{
	if(!good()) {
		spdlog::error("Mesh can't load");
		std::exit(1);
	}
	std::vector<StaticMesh> meshes;
	spdlog::warn("Mesh count: {}", pScene->mNumMeshes);
	tcb::span<aiMesh *> meshesInFile(pScene->mMeshes,pScene->mNumMeshes);
	for(const auto& paiMesh: meshesInFile) {
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
SkeletalMesh AssetImporter::loadSkeletalMesh()
{

	SkeletalMesh skeletalMeshes;
	tcb::span<aiMesh *> meshesInFile(pScene->mMeshes,pScene->mNumMeshes);
	int32 clk = 0;
	for(const auto& paiMesh: meshesInFile) {
		spdlog::warn("{} - {}", clk, pScene->mMaterials[paiMesh->mMaterialIndex]->GetName().C_Str());
			clk++;
		auto skeletalMeshElement = std::make_shared<SkeletalMeshElement>();
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			const aiVector3D *pPos = &(paiMesh->mVertices[i]);
			const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
			skeletalMeshElement->vertices.emplace_back(pPos->x, pPos->y, pPos->z);
			skeletalMeshElement->normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
			skeletalMeshElement->textureCoordinates.emplace_back(pTexCoord->x, pTexCoord->y);
		}

		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
		{
			const aiFace &Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			skeletalMeshElement->indices.push_back(Face.mIndices[0]);
			skeletalMeshElement->indices.push_back(Face.mIndices[1]);
			skeletalMeshElement->indices.push_back(Face.mIndices[2]);
		}
		skeletalMeshElement->updateRenderData();
		skeletalMeshes.addSkeletalMeshElement(skeletalMeshElement);
	}
	return skeletalMeshes;
}
