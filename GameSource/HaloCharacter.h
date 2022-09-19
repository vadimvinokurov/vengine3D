//
// Created by boris on 20.09.2022.
//

#ifndef VENGINE3D_HALOCHARACTER_H
#define VENGINE3D_HALOCHARACTER_H

#include "BaseCharacter.h"
#include "AssetImporter.h"

class HaloCharacter : public BaseCharacter
{
	GENERATE_ENTITY_BODY(BaseCharacter)
public:
	void onCreate() override
	{
		Super::onCreate();

		AssetImporter haloAsset = AssetImporter("../Content/Mesh/halo/source/halo.fbx");

		auto skeletalMeshModel = std::make_shared<SkeletalMeshModel>(haloAsset.getSkeletalMesh());
		skeletalMeshComponent->skeletalMesh.setSkeletalMeshModel(skeletalMeshModel);

		std::vector<ShaderSource> shaderSource;
		shaderSource.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
		shaderSource.emplace_back("../Content/Shaders/animdefault.vert", ShaderType::Vertex);

		skeletalMeshComponent->skeletalMesh.materials[0] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[1] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Chest_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[2] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Arms_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[3] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Legs_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[4] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[5] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[6] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Ears_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[7] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Helmet_Mat_BaseColor.png", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[8] =
			std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Ears_Mat_BaseColor.png", shaderSource);

		Skeleton restPose = haloAsset.getSkeleton();
		auto animations = haloAsset.getAnimations();

		skeletalMeshComponent->skeleton = std::make_shared<Skeleton>(restPose);

		skeletalMeshComponent->animation = std::make_shared<Animation>(animations.begin()->second);
		skeletalMeshComponent->animation->setLooping(true);
	}
};

#endif // VENGINE3D_HALOCHARACTER_H
