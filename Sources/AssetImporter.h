//
// Created by boris on 8/19/2022.
//

#ifndef VENGINE3D_ASSETIMPORTER_H
#define VENGINE3D_ASSETIMPORTER_H

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "StaticMesh.h"
#include <memory>

class SkeletalMesh;

class AssetImporter {
public:
	AssetImporter(const char* path);
	bool good();
	std::vector<StaticMesh> loadMeshes();


	SkeletalMesh loadSkeletalMesh();
private:
	const aiScene* pScene = nullptr;
	Assimp::Importer Importer;
};


#endif // VENGINE3D_ASSETIMPORTER_H
